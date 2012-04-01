/*
 * QrDecoder.cpp
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */
#include <opencv2/imgproc/imgproc.hpp>

#include "QrDecoder.h"
#include "QrVersionInformation.h"
#include "QrFormatInformation.h"
#include "../../common/miscellaneous.h"
#include "../../common/Line2D.h"
#include "../../common/Polygon2D.h"
#include "../../debug.h"
#include "bitdecoder/QrBitDecoder.h"
#include "QrCodewordsCharacteristics.h"

#define DEBUG_TAG "QrDecoder.cpp"

namespace barcodes {

const QrDecoder QrDecoder::DECODER_INSTANCE = QrDecoder();

const Size QrDecoder::CODEWORD_SAMPLE_SIZE(2, 4);

void QrDecoder::decode(Image &image, vector<DataSegment> &dataSegments, int flags) const {
	DetectedMarks detectedMarks;

	dataSegments.clear();
	QrDetector::getInstance()->detect(image, detectedMarks, flags);

	if (detectedMarks.size() == 3) {
		_read_V2_40(image, dataSegments, detectedMarks);
	}
}

const QrDecoder *QrDecoder::getInstance() {
	return &DECODER_INSTANCE;
}

const Decoder *QrDecoder::getDecoder() const {
	return &DECODER_INSTANCE;
}

void QrDecoder::_read_V2_40(Image &image, vector<DataSegment> &dataSegments, DetectedMarks &detectedMarks, int flags) const {
	vector<Point> corners;
	DetectedMarks _detectedMarks = detectedMarks;
	Mat binarized = QrDetector::binarize(image, _detectedMarks[0].flags);
	getPerspectiveCorners_V1_40(binarized, _detectedMarks, corners);

	Image img = Image(image.clone(), IMAGE_COLOR_GRAYSCALE);
	cvtColor(img, img, CV_GRAY2RGB);

	line( img, corners[0],corners[0], CV_RGB(255, 0, 0), 2, 8, 0);
	line( img, corners[1],corners[1], CV_RGB(0, 255, 0), 2, 8, 0);
	line( img, corners[2],corners[2], CV_RGB(0, 0, 255), 2, 8, 0);
	line( img, corners[3],corners[3], CV_RGB(255, 255, 0), 2, 8, 0);
	DEBUG_WRITE_IMAGE("out2/just_test.jpg", img);

	if (corners.size() < 4) return;

	Mat perspWarped = warpPerspective(binarized, corners, false, Size(QR_CODE_WARP_PERSPECTIVE_SIZE, QR_CODE_WARP_PERSPECTIVE_SIZE));
	Mat transformation = getPerspectiveTransform(corners, Size(QR_CODE_WARP_PERSPECTIVE_SIZE, QR_CODE_WARP_PERSPECTIVE_SIZE));
	_detectedMarks.perspectiveTransform(transformation);
	DEBUG_WRITE_IMAGE("out2/warped.jpg", perspWarped);

	// GETTING THE QR CODE VERSION FROM THE IMAGE

	QrVersionInformation versionInformation = QrVersionInformation::fromImage(perspWarped, _detectedMarks);

	if (versionInformation == QrVersionInformation::INVALID_VERSION) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET VERSION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "VERSION: %d", versionInformation.getVersion());

	// TRANSLATING THE IMAGE TO THE BIT MATRIX

	BitMatrix qrBitMatrix;
	BitMatrix::fromImage(perspWarped, versionInformation.getQrBarcodeSize(), qrBitMatrix);

	if (qrBitMatrix.data == NULL) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET QR BIT MATRIX!");
		return;
	}

	// GETTING THE FORMAT INFORMATION FROM THE BIT MATRIX

	QrFormatInformation formatInformation = QrFormatInformation::fromBitMatrix(qrBitMatrix, versionInformation);

	if (formatInformation == QrFormatInformation::INVALID_FORMAT) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET FORMAT INFORMATION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "XOR DATA MASK: %d | ERROR CORRECTION LEVEL: %d", formatInformation.getXORDataMask(), formatInformation.getErrorCorrectionLevel());

	// BUILDING THE XOR DATA MASK AND MASKING THE QR CODE BIT MATRIX

	BitMatrix xorDataMask;
	formatInformation.buildXORDataMask(xorDataMask, versionInformation);

	qrBitMatrix.maskXOR(xorDataMask);

	// SAMPLING THE QR CODE AND RETRIEVING BIT ARRAY CONTATINING DATA AND ERROR CORRECTION CODEWORDS

	GridSampler sampler(CODEWORD_SAMPLE_SIZE, GridSampler::LEFT_TOP, GridSampler::TOP_LEFT, false, true);
	BitMatrix dataMask;
	BitArray qrDataErrorBits;

	versionInformation.getDataMask(dataMask);
	qrBitMatrix.removeCol(6);
	dataMask.removeCol(6);
	sampler.sample(qrBitMatrix, qrDataErrorBits, &dataMask);
	DEBUG_PRINT(DEBUG_TAG, "READ DATA/ERROR BITS: %d", qrDataErrorBits.size());

	// DIVIDE BITARRAY INTO CODEWORDS AND RE-ORDER, RETURNS ORDERED BITARRAY
	QrVersionFormatCharacteristics characteristics;
	QrCodewordsCharacteristics::getInstance().getCharacteristics(versionInformation, formatInformation, characteristics);

	// PROCEED THE ERROR CORRECTION
	// - TODO

	// FINALLY DECODE THE DATA

	QrBitDecoder::getInstance().decode(qrDataErrorBits, dataSegments, versionInformation);
	if (dataSegments.size() == 0) return;


	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT length: %d", dataSegments[0].data.size());
	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT: %s", (dataSegments[0].data.size() > 0)? string((const char *)&dataSegments[0].data[0], dataSegments[0].data.size()).c_str() : NULL);
	DEBUG_PRINT_BITVECTOR(DEBUG_TAG, qrDataErrorBits);
	DEBUG_WRITE_BITMATRIX("out2/data_mask.bmp", dataMask);
	DEBUG_WRITE_BITMATRIX("out2/data_masked.bmp", qrBitMatrix);

}

// Getting the A,B,C,D points, sorted as C,D,A,B for perspective transformation
// A,B,C are retrieved from the X,Y,Z
// Y is the point with the maximal inner distance from the polygon (triangle)
// which is made by the center points of QR marks.
//
// B******           ******C
// * *** *           * *** *
// * *** *-----------* *** *
// * *** *           * *** *
// ******Y           Z*****N
//    |            /
//    |         /
//    |       /
//    |    /
// ******X
// * *** *
// * *** *
// * *** *
// A*****M                 D
void QrDecoder::getPerspectiveCorners_V1_40(Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const {
	corners.clear();

	//============= DETERMINING THE THREE CORNERS (A, B, C) FROM THE DETECTED MARKS

	// Sort each points of detected marks clockwise
	vector<Point> *mark1_points = &detectedMarks[0].points;
	convexHull(Mat(*mark1_points), *mark1_points);
	vector<Point> *mark2_points = &detectedMarks[1].points;
	convexHull(Mat(*mark2_points), *mark2_points);
	vector<Point> *mark3_points = &detectedMarks[2].points;
	convexHull(Mat(*mark3_points), *mark3_points);

	// Get center of each mark
	Point mark1_center;
	Line2D::intersection(Line2D((*mark1_points)[0], (*mark1_points)[2]), Line2D((*mark1_points)[1], (*mark1_points)[3]), mark1_center);
	Point mark2_center;
	Line2D::intersection(Line2D((*mark2_points)[0], (*mark2_points)[2]), Line2D((*mark2_points)[1], (*mark2_points)[3]), mark2_center);
	Point mark3_center;
	Line2D::intersection(Line2D((*mark3_points)[0], (*mark3_points)[2]), Line2D((*mark3_points)[1], (*mark3_points)[3]), mark3_center);

	// Get sorted points of the polygon made by the centers of marks
	vector<Point> mark_centers_disorted, mark_centers;
	mark_centers_disorted.push_back(mark1_center);
	mark_centers_disorted.push_back(mark2_center);
	mark_centers_disorted.push_back(mark3_center);
	convexHull(Mat(mark_centers_disorted), mark_centers);

	// Changing position inside detectedMarks due to previous sorting of the centers
	// Detected marks will be sorted clock-wise
	vector<Point>::iterator iter;
	DetectedMarks _detectedMarks;
	for (iter = mark_centers.begin(); iter != mark_centers.end(); iter++) {
		if (*iter == mark1_center) {
			_detectedMarks.push_back(detectedMarks[0]);
		} else if (*iter == mark2_center) {
			_detectedMarks.push_back(detectedMarks[1]);
		} else {
			_detectedMarks.push_back(detectedMarks[2]);
		}
	}
	detectedMarks = _detectedMarks;

	// Pointers to points has be changed too (positions has changed)
	mark1_points = &detectedMarks[0].points;
	mark2_points = &detectedMarks[1].points;
	mark3_points = &detectedMarks[2].points;

	// Getting the positions of the nearest points from the contour mark_centers, prefers points inside
	int mark1_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark1_points, true);
	int mark2_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark2_points, true);
	int mark3_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark3_points, true);

	// Getting nearest point distance from the contour mark_centers
	double mark1_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark1_points)[mark1_nearest_point_position], true);
	double mark2_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark2_points)[mark2_nearest_point_position], true);
	double mark3_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark3_points)[mark3_nearest_point_position], true);

	// Rotating points that the most outer point will the first
	rotate((*mark1_points).begin(), (*mark1_points).begin() + (mark1_nearest_point_position + 2) % 4, (*mark1_points).end());
	rotate((*mark2_points).begin(), (*mark2_points).begin() + (mark2_nearest_point_position + 2) % 4, (*mark2_points).end());
	rotate((*mark3_points).begin(), (*mark3_points).begin() + (mark3_nearest_point_position + 2) % 4, (*mark3_points).end());

	// Sorting the detected marks too. First mark will be now left bottom and next clock-wise.
	if ((mark1_nearest_point_distance >= mark2_nearest_point_distance) && (mark1_nearest_point_distance >= mark3_nearest_point_distance)) {
		rotate(detectedMarks.begin(),detectedMarks.begin() + 2,detectedMarks.end());
	} else if ((mark3_nearest_point_distance >= mark2_nearest_point_distance) && (mark3_nearest_point_distance >= mark1_nearest_point_distance)) {
		rotate(detectedMarks.begin(),detectedMarks.begin() + 1,detectedMarks.end());
	}

	//============= DETERMINING THE FROUTH UNKNOWN CORNER (D) BY RECT SAMPLING

	// Width of the sampling rectangle will be the approximate size of the one dot divided by 2
	RotatedRect box = minAreaRect(Mat(detectedMarks[0].points));
	Size2f boxSize = box.size;
	int sampleLineWidth = (boxSize.width > boxSize.height)? ceil(boxSize.width / (double)14) : ceil(boxSize.height / (double)14);

	// The point around which will be sample rectangle rotated (point M)
	Point2f rotatePoint = detectedMarks[0].points[3];

	// The start point from which sampling begins, its distance is equal to distance
	// between the "center" of the QR code and is shifted SAMPLE_RECT_START_SHIFT degrees inside the QR code
	Vector2Df diagVector(detectedMarks[0].points[0], detectedMarks[2].points[0]);
	Vector2Df sampleVector(detectedMarks[0].points[0], detectedMarks[0].points[3]);
	Vector2Df lineShift(detectedMarks[0].points[2], detectedMarks[0].points[3]);
	lineShift.resize(sampleLineWidth / 2);
	sampleVector.rotate(SAMPLE_RECT_START_ANGLE_SHIFT);
	sampleVector.resize(diagVector.size() / 2);

	sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, sampleLineWidth, true);

	// Now find the second edge
	Point2f rotatePoint2 = detectedMarks[2].points[1];
	Vector2Df sampleVector2(detectedMarks[2].points[0], detectedMarks[2].points[1]);
	lineShift = Vector2Df(detectedMarks[2].points[2], detectedMarks[2].points[1]);
	lineShift.resize(sampleLineWidth / 2);
	sampleVector2.rotate(SAMPLE_RECT_START_ANGLE_SHIFT * -1);
	sampleVector2.resize(diagVector.size() / 2);

	sampleQrCodeEdge(binarized, sampleVector2, rotatePoint2, lineShift, sampleLineWidth, false);

	// Finally finding the 4th corner from the intersection of the lines
	Point fourthCorner;
	Line2D::intersection(Line2D(rotatePoint, sampleVector + rotatePoint), Line2D(rotatePoint2, sampleVector2 + rotatePoint2), fourthCorner);

	// The outer point we will push as a result perspective points
	corners.push_back(detectedMarks[2].points[0]);
	corners.push_back(fourthCorner);
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);
}

bool QrDecoder::sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample) const {
	double sampleAngleStep = (clockWiseSample)? SAMPLE_RECT_ANGLE_STEP : SAMPLE_RECT_ANGLE_STEP * -1;
	double angle = 0;

	while (fabs(angle) < SAMPLE_RECT_MAX_ANGLE) {

		double sampleRatio = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        if (sampleRatio < SAMPLE_RECT_EDGE_DETECT_FILL_RATIO) {
        	bool wasLeft = false;
        	Vector2Df finalSampleVector = sampleVector;
        	int finalStep = 0;

        	// Going one step back
        	sampleVector.rotate(sampleAngleStep);
        	angle -= sampleAngleStep;

        	// Final steps procedure
        	while (finalStep < SAMPLE_RECT_FINAL_STEPS_COUNT) {
    			DEBUG_PRINT(DEBUG_TAG, "FINAL STEPS");
        		sampleAngleStep = sampleAngleStep * 0.5; // Divide sample step by 2

        		sampleVector.rotate(sampleAngleStep * -0.5); // 1/4 forward (to 1/4 position)
        		double sampleRatioLeft = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		sampleVector.rotate(sampleAngleStep * -1); // 1/2 forward (to 3/4 position)
        		double sampleRatioRight = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		if (sampleRatioLeft < sampleRatioRight) {
        			wasLeft = true;
        			DEBUG_PRINT(DEBUG_TAG, "WAS LEFT");
            		sampleVector.rotate(sampleAngleStep * 1.5); // 3/4 back (to begin)
        		} else {
            		sampleVector.rotate(sampleAngleStep * 0.5); // 1/4 back (to center)
        		}

        		finalStep++;
        	}

        	sampleVector = (wasLeft)? finalSampleVector : sampleVector;
        	return true;
        }

        DEBUG_PRINT(DEBUG_TAG, "Before rotate: %d ; %d", sampleVector.dx, sampleVector.dy);
		sampleVector.rotate(sampleAngleStep * -1);
        DEBUG_PRINT(DEBUG_TAG, "After rotate: %d ; %d", sampleVector.dx, sampleVector.dy);
		angle += sampleAngleStep;
	}

	DEBUG_PRINT(DEBUG_TAG, "REACHED THE END");
	return false;
}

double QrDecoder::_sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) const {
	MatND hist;

	// Getting the hull mask and cropping the contour image
	Mat sampleMask = Mat::zeros(Size(binarized.cols, binarized.rows), CV_8UC1);
	line(sampleMask, lineShift + rotatePoint, sampleVector + lineShift + rotatePoint, CV_RGB(255, 255, 255), lineWidth, 8, 0);

	calcBinarizedHistogram(binarized, sampleMask, hist);
	float fill_density = hist.at<float>(0);
    float bg_density = hist.at<float>(1);

    DEBUG_PRINT(DEBUG_TAG, "BG: %.4f || FILL: %.4f", bg_density, fill_density);

    return fill_density / (double)(fill_density + bg_density);
}

} /* namespace barcodes */

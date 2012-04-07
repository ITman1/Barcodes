///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDecoder.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDecoder class which implements decoder
//             of the QR codes in the image.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDecoder.cpp
 *
 * @brief Defines members of QrDecoder class which implements decoder
 *        of the QR codes in the image.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>

#include "QrDecoder.h"
#include "QrVersionInformation.h"
#include "QrFormatInformation.h"
#include "../../common/miscellaneous.h"
#include "../../common/Line2D.h"
#include "../../common/Polygon2D.h"
#include "../../debug.h"
#include "bitdecoder/QrBitDecoder.h"
#include "QrCodewordOrganizer.h"

#define DEBUG_TAG "QrDecoder.cpp"

namespace barcodes {

/**
 * Instance of the QR decoder.
 */
const QrDecoder QrDecoder::DECODER_INSTANCE = QrDecoder();

/**
 * The size of the sampling grid for retrieving the data from the bit matrix.
 */
const Size QrDecoder::CODEWORD_SAMPLE_SIZE(2, 4);

/**
 * Decodes QR code on the image and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param dataSegments Result decoded data segments.
 * @param flags Flags used for detection and decoding.
 */
void QrDecoder::decode(Image &image, DataSegments &dataSegments, int flags) const {
	DetectedMarks detectedMarks;


	QrDetector::getInstance()->detect(image, detectedMarks, flags);

	if (detectedMarks.size() == 3) {
		_read_V1_40(image, dataSegments, detectedMarks);
	}
}

/**
 * Returns instance of the QR decoder.
 *
 * @return Instance of the QR decoder.
 */
const QrDecoder *QrDecoder::getInstance() {
	return &DECODER_INSTANCE;
}

/**
 * Decodes QR code of the versions 1-40 and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param dataSegments Result decoded data segments.
 * @param detectedMarks The localization marks.
 * @param flags Flags used for detection and decoding.
 */
void QrDecoder::_read_V1_40(Image &image, DataSegments &dataSegments, DetectedMarks &detectedMarks, int flags) const {
	dataSegments.clear();
	dataSegments.flags = 0;

	//>>> 1) GETTING THE FOUR CORNERS OF THE QR CODE FROM THE IMAGE

	vector<Point> corners;
	DetectedMarks _detectedMarks = detectedMarks;
	Mat binarized = QrDetector::binarize(image, _detectedMarks[2].flags);
	getPerspectiveCorners_V1_40(binarized, _detectedMarks, corners);

	if (corners.size() < 4) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET FOUR CORNERS OF THE QR CODE!");
		return;
	}

#ifdef TARGET_DEBUG
	Image img;
#endif
	DEBUG_WRITE_IMAGE(
		(
				img = Image(image.clone(), IMAGE_COLOR_GRAYSCALE),
				cvtColor(img, img, CV_GRAY2RGB),
				line( img, corners[0],corners[0], CV_RGB(255, 0, 0), 5, 8, 0),
				line( img, corners[1],corners[1], CV_RGB(0, 255, 0), 5, 8, 0),
				line( img, corners[2],corners[2], CV_RGB(0, 0, 255), 5, 8, 0),
				line( img, corners[3],corners[3], CV_RGB(255, 255, 0), 5, 8, 0),
				"perspective_corners.jpg"
		), img);

	//>>> 2) PERSPECTIVE TRANSFORMATION OF THE QR CODE

	int warpPerspectiveSize = (binarized.cols > binarized.rows)? binarized.cols : binarized.rows;
	Mat perspWarped = warpPerspective(binarized, corners, false, Size(warpPerspectiveSize, warpPerspectiveSize));
	perspWarped = QrDetector::binarize(perspWarped, QrDetector::FLAG_GLOBAL_THRESH);
	Mat transformation = getPerspectiveTransform(corners, Size(warpPerspectiveSize, warpPerspectiveSize));
	_detectedMarks.perspectiveTransform(transformation);
	DEBUG_WRITE_IMAGE("warped.jpg", perspWarped);

	//>>> 3) GETTING THE QR CODE VERSION FROM THE IMAGE

	QrVersionInformation versionInformation = QrVersionInformation::fromImage(perspWarped, _detectedMarks);

	if (versionInformation == QrVersionInformation::INVALID_VERSION) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET VERSION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "Processing version: %d", versionInformation.getVersion());

	//>>> 4) TRANSLATING THE IMAGE TO THE BIT MATRIX

	BitMatrix qrBitMatrix;
	BitMatrix::fromImage(perspWarped, versionInformation.getQrBarcodeSize(), qrBitMatrix);

	if (qrBitMatrix.data == NULL) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET QR BIT MATRIX!");
		return;
	}
	DEBUG_WRITE_BITMATRIX("data_unmasked.bmp", qrBitMatrix);

	//>>> 5) GETTING THE FORMAT INFORMATION FROM THE BIT MATRIX

	QrFormatInformation formatInformation = QrFormatInformation::fromBitMatrix(qrBitMatrix, versionInformation);

	if (formatInformation == QrFormatInformation::INVALID_FORMAT) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET FORMAT INFORMATION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "XOR DATA MASK: %d | ERROR CORRECTION LEVEL: %d", formatInformation.getXORDataMask(), formatInformation.getErrorCorrectionLevel());

	//>>> 6) BUILDING THE XOR DATA MASK AND MASKING THE QR CODE BIT MATRIX

	BitMatrix xorDataMask;
	formatInformation.buildXORDataMask(xorDataMask, versionInformation);

	qrBitMatrix.maskXOR(xorDataMask);
	DEBUG_WRITE_BITMATRIX("xor_mask.bmp", xorDataMask);
	DEBUG_WRITE_BITMATRIX("data_masked.bmp", qrBitMatrix);

	//>>> 7) SAMPLING THE QR CODE AND RETRIEVING BIT ARRAY CONTATINING DATA AND ERROR CORRECTION CODEWORDS

	GridSampler sampler(CODEWORD_SAMPLE_SIZE, GridSampler::LEFT_TOP, GridSampler::TOP_LEFT, false, true);
	BitMatrix dataMask;
	BitArray qrDataErrorBits;

	versionInformation.getDataMask(dataMask);
	qrBitMatrix.removeCol(6);
	dataMask.removeCol(6);
	sampler.sample(qrBitMatrix, qrDataErrorBits, &dataMask);
	DEBUG_PRINT(DEBUG_TAG, "READ DATA/ERROR BITS: %d", qrDataErrorBits.size());
	DEBUG_WRITE_BITMATRIX("data_mask.bmp", dataMask);

	//>>> 8) DIVIDES BITARRAY INTO CODEWORDS, RE-ORDERS AND RETURNS ORDERED BLOCKS CONTAINING EC PARITY BITS

	QrCodewordOrganizer codewordOrganizer(versionInformation, formatInformation);
	vector<BitArray> blocks;
	codewordOrganizer.extractBlocks(qrDataErrorBits, blocks);

	//>>> 9) PROCEEDS THE ERROR CORRECTION AND EXTRACTS CORECTED CODEWORDS

	BitArray codewords;
	if (!codewordOrganizer.correctBlocks(blocks)) {
		dataSegments.flags |= DataSegments::DATA_SEGMENTS_CORRUPTED;
		DEBUG_PRINT(DEBUG_TAG, "BLOCKS ARE CORRUPTED!");
	}
	codewordOrganizer.blocksToCodewords(blocks, codewords);

	//>>> 10) FINALLY DECODE THE DATA

	QrBitDecoder::getInstance().decode(codewords, dataSegments, versionInformation);
	DEBUG_PRINT(DEBUG_TAG, "DATA SEGMENT COUNT: %d", dataSegments.size());
	if (dataSegments.size() == 0)  {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET DATA SEGMENTS!");
		return;
	}

	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT length: %d", dataSegments[0].data.size());
	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT mode: %d", dataSegments[0].mode);
	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT: %s", (dataSegments[0].data.size() > 0)? string((const char *)&dataSegments[0].data[0], dataSegments[0].data.size()).c_str() : NULL);
}

/**
 * Retrieves four corners for perspective transformation and sorts the
 * detected marks and theirs points as defined bellow.
 *
 * \code
 *  Getting the A,B,C,D points, sorted as C,D,A,B for perspective transformation
 *
 *  A,B,C are retrieved from the X,Y,Z
 *
 *  X,Y,Z are the points with the minimal inner/outer distance from the polygon (triangle)
 *  which is made by the center points of QR marks.
 *
 *  D point is sampled by two lines which has rotate pivot in the N and M.
 *  These lines start sampling inside QR code and goes outside and when there is
 *  a white background they ends. D is calculated as an intersect of these lines.
 *
 *  B******           ******C
 *  * *** *           * *** *
 *  * *** *-----------* *** *
 *  * *** *           * *** *
 *  ******Y           Z*****N
 *     |            /
 *     |         /
 *     |       /
 *     |    /
 *  ******X
 *  * *** *
 *  * *** *
 *  * *** *
 *  A*****M                 D
 * \endcode
 *
 * @param binarized Binarized image with the QR code.
 * @param detectedMarks Detected marks. These marks are sorted.
 * @param corners Result corners for perspective transformation.
 */
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

	if (!sampleQrCodeEdge(binarized, sampleVector2, rotatePoint2, lineShift, sampleLineWidth, false)) {
		return;
	}

	// Finally finding the 4th corner from the intersection of the lines
	Point fourthCorner;
	Line2D::intersection(Line2D(rotatePoint, sampleVector + rotatePoint), Line2D(rotatePoint2, sampleVector2 + rotatePoint2), fourthCorner);

	// The outer point we will push as a result perspective points
	corners.push_back(detectedMarks[2].points[0]);
	corners.push_back(fourthCorner);
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);
}

/**
 * Proceeds sampling by rectangle.
 *
 * @param binarized Binarized image.
 * @param sampleVector The vector which defines the direction and sizes of the sample rectangle.
 * @param rotatePoint Point around which should be rotated the sample rectangle.
 * @param lineShift Offset of the sample rectangle.
 * @param lineWidth Width of the sample rectangle.
 * @param clockWiseSample Direction of the sampling.
 * @return Returns true if maximal sample rotation angle is not exceeded, otherwise false.
 */
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
        		sampleAngleStep = sampleAngleStep * 0.5; // Divide sample step by 2

        		sampleVector.rotate(sampleAngleStep * -0.5); // 1/4 forward (to 1/4 position)
        		double sampleRatioLeft = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		sampleVector.rotate(sampleAngleStep * -1); // 1/2 forward (to 3/4 position)
        		double sampleRatioRight = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		if (sampleRatioLeft < sampleRatioRight) {
        			wasLeft = true;
            		sampleVector.rotate(sampleAngleStep * 1.5); // 3/4 back (to begin)
        		} else {
            		sampleVector.rotate(sampleAngleStep * 0.5); // 1/4 back (to center)
        		}

        		finalStep++;
        	}

        	sampleVector = (wasLeft)? finalSampleVector : sampleVector;
        	return true;
        }

		sampleVector.rotate(sampleAngleStep * -1);
		angle += sampleAngleStep;
	}

	DEBUG_PRINT(DEBUG_TAG, "DETECTION OF THE 4. CORNER FAILED!");
	return false;
}

/**
 * Proceeds one sample and returns background fill ratio.
 *
 * @param binarized Binarized image.
 * @param sampleVector The vector which defines the direction and sizes of the sample rectangle.
 * @param rotatePoint Point around which should be rotated the sample rectangle.
 * @param lineShift Offset of the sample rectangle.
 * @param lineWidth Width of the sample rectangle.
 * @return Fill ratio of the background.
 */
double QrDecoder::_sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) const {
	MatND hist;

	// Getting the hull mask and cropping the contour image
	Mat sampleMask = Mat::zeros(Size(binarized.cols, binarized.rows), CV_8UC1);
	line(sampleMask, lineShift + rotatePoint, sampleVector + lineShift + rotatePoint, CV_RGB(255, 255, 255), lineWidth, 8, 0);

	calcBinarizedHistogram(binarized, sampleMask, hist);
	float fill_density = hist.at<float>(0);
    float bg_density = hist.at<float>(1);

    return fill_density / (double)(fill_density + bg_density);
}

} /* namespace barcodes */

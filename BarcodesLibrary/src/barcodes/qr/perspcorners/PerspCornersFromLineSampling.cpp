///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersFromLineSampling.cpp
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines functor class for retrieving the perspective corners
//             of the QR code in the image using line sampling for that.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersFromLineSampling.cpp
 *
 * @brief Defines functor class for retrieving the perspective corners
 * of the QR code in the image using line sampling for that.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "PerspCornersHelper.h"
#include "PerspCornersFromLineSampling.h"
#include "../../../debug.h"
#include "../../../common/Line2D.h"

#define DEBUG_TAG "PerspCornersFromLineSampling.cpp"

namespace barcodes {

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
bool PerspCornersFromLineSampling::sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample) const {
	DEBUG_PRINT(DEBUG_TAG, "sampleQrCodeEdge()");

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
double PerspCornersFromLineSampling::_sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) const {
	Mat transMat;
	Point2f src[3];
	Point2f dst[3];
	double sampleSize = sampleVector.size();

	src[0] = rotatePoint;
	src[1] = lineShift * 2 + rotatePoint;
	src[2] = sampleVector + rotatePoint;

	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(0, lineWidth);
	dst[2] = Point2f(sampleSize, 0);

	transMat = getAffineTransform(src, dst);

	warpAffine(binarized, transformed, transMat, Size(sampleSize, lineWidth));

	int bg_density = cv::countNonZero(transformed);

    return (sampleSize * lineWidth - bg_density) / (double)(sampleSize * lineWidth);

	/*MatND hist;

	// Getting the hull mask and cropping the contour image
	Mat sampleMask = Mat::zeros(Size(binarized.cols, binarized.rows), CV_8UC1);
	line(sampleMask, lineShift + rotatePoint, sampleVector + lineShift + rotatePoint, CV_RGB(255, 255, 255), lineWidth, 8, 0);

	calcBinarizedHistogram(binarized, sampleMask, hist);
	float fill_density = hist.at<float>(0);
    float bg_density = hist.at<float>(1);

    return fill_density / (double)(fill_density + bg_density);*/
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
void PerspCornersFromLineSampling::getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const {

	//============= DETERMINING THE THREE CORNERS (A, B, C) FROM THE DETECTED MARKS

	corners.clear();
	if (!PerspCornersHelper::sortDetectedMarks(detectedMarks)) {
		return;
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

	Vector2Df _sampleVector = sampleVector;
	if (!sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, sampleLineWidth, true)) {
		sampleVector = _sampleVector;
	}

	// Now find the second edge
	Point2f rotatePoint2 = detectedMarks[2].points[1];
	Vector2Df sampleVector2(detectedMarks[2].points[0], detectedMarks[2].points[1]);
	lineShift = Vector2Df(detectedMarks[2].points[2], detectedMarks[2].points[1]);
	lineShift.resize(sampleLineWidth / 2);
	sampleVector2.rotate(SAMPLE_RECT_START_ANGLE_SHIFT * -1);
	sampleVector2.resize(diagVector.size() / 2);

	_sampleVector = sampleVector;
	if (!sampleQrCodeEdge(binarized, sampleVector2, rotatePoint2, lineShift, sampleLineWidth, false)) {
		sampleVector = _sampleVector;
	}

	// Finally finding the 4th corner from the intersection of the lines
	Point fourthCorner;
	if (!Line2D::intersection(Line2D(rotatePoint, sampleVector + rotatePoint),
			Line2D(rotatePoint2, sampleVector2 + rotatePoint2), fourthCorner)) {
		return;
	}

	// The outer point we will push as a result perspective points
	corners.push_back(detectedMarks[2].points[0]);
	corners.push_back(fourthCorner);
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);
}

} /* namespace barcodes */

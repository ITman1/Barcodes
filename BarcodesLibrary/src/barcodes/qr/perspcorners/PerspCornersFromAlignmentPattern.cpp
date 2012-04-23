///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersFromFinderPattern.cpp
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines functor class for retrieving the perspective corners
//             of the QR code in the image using the alignment pattern for that.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersFromFinderPattern.cpp
 *
 * @brief Defines functor class for retrieving the perspective corners
 * of the QR code in the image using the alignment pattern for that.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "PerspCornersFromAlignmentPattern.h"
#include "PerspCornersFromFinderPattern.h"
#include "../../../common/miscellaneous.h"
#include "../QrDetector.h"
#include "../QrVersionInformation.h"
#include "../QrBuildHelper.h"
#include "PerspCornersHelper.h"

#include <opencv2/highgui/highgui.hpp>

namespace barcodes {

static PerspCornersFromFinderPattern perspFromFinderPattern;

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
 *  D point is retrieved from the right/bottom most alignment pattern.
 *
 *  B******           ******C
 *  * *** *           * *** *
 *  * *** *-----------* *** *
 *  * *** *           * *** *
 *  ******Y           Z*****N
 *     |    .       /
 *     |       . /
 *     |       / .
 *     |    /      .
 *  ******X          .
 *  * *** *           * * *
 *  * *** *           * * *
 *  * *** *           * * *
 *  A*****M                 D
 * \endcode
 *
 * @param binarized Binarized image with the QR code.
 * @param detectedMarks Detected marks. These marks are sorted.
 * @param corners Result corners for perspective transformation.
 */
void PerspCornersFromAlignmentPattern::getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const {
	corners.clear();
	DetectedMarks _detectedMarks = detectedMarks;

	// For getting the version we have to warp image somehow, here we use
	// PerspFromFinderPattern to approximate the perspective points

	perspFromFinderPattern.getPerspectiveCorners(image, binarized, _detectedMarks, corners);

	if (corners.size() < 4) {
		corners.clear();
		return;
	}

	int warpPerspectiveSize = (binarized.cols > binarized.rows)? binarized.cols : binarized.rows;
	Mat warpedImage = warpPerspective(binarized, corners, false, Size(warpPerspectiveSize, warpPerspectiveSize));
	warpedImage = QrDetector::binarize(warpedImage, QrDetector::FLAG_ADAPT_THRESH | QrDetector::FLAG_DISTANCE_NEAR);

	// Getting the version which we actual need for getting the position of the alignment mark and barcode size

	QrVersionInformation versionInformation = QrVersionInformation::fromImage(warpedImage, _detectedMarks);
	if (versionInformation == QrVersionInformation::INVALID_VERSION) {
		corners.clear();
		return;
	}

	vector<Rect> alignementPatternsPositions;
	versionInformation.getAlignmentPatternPositions(alignementPatternsPositions);

	if (alignementPatternsPositions.empty()) {
		corners.clear();
		return;
	}
	Rect mostRightBottomAlignmentRect = alignementPatternsPositions.back();

	// Matching the right buttom alignment mark in the image, we use only cropped part of the image

	int barcodeSize = versionInformation.getQrBarcodeSize().width;
	double moduleSize = warpPerspectiveSize / (double)versionInformation.getQrBarcodeSize().width;
	Point cropPosition = Point((mostRightBottomAlignmentRect.x - 6) * moduleSize, (mostRightBottomAlignmentRect.y - 6) * moduleSize);

	Mat templateImage = QrBuildHelper::buildAlignementMark(moduleSize * 5);
	Point matchLoc;
	matchTemplate(warpedImage(Rect(cropPosition, Point(warpedImage.cols, warpedImage.rows))), templateImage, CV_TM_SQDIFF_NORMED, matchLoc);
	matchLoc = cropPosition + matchLoc;

	Point rightBottomCorner;
	rightBottomCorner.x = (matchLoc.x / (double)mostRightBottomAlignmentRect.x) * (double)barcodeSize;
	rightBottomCorner.y = (matchLoc.y / (double)mostRightBottomAlignmentRect.y) * (double)barcodeSize;

	// Transforming the fourth point back to the perspective projection

	vector<Point> srcCorners;
	srcCorners.push_back(Point(warpPerspectiveSize, 0));
	srcCorners.push_back(Point(warpPerspectiveSize, warpPerspectiveSize));
	srcCorners.push_back(Point(0, warpPerspectiveSize));
	srcCorners.push_back(Point(0, 0));
	Mat transformation = getPerspectiveTransform(srcCorners, corners);

	vector<Point2f> points2f;
	points2f.push_back(Point2f(rightBottomCorner.x, rightBottomCorner.y));

	Mat res;
	cv::perspectiveTransform(Mat(points2f), res, transformation);

	if (!PerspCornersHelper::sortDetectedMarks(detectedMarks)) {
		return;
	}

	// The outer point we will push as a result perspective points

	corners.clear();
	corners.push_back(detectedMarks[2].points[0]);
	corners.push_back(res.at<Point2f>(0, 0));
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);


}
} /* namespace barcodes */

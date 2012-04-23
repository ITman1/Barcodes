///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersFromFinderPattern.cpp
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines functor class for retrieving the perspective corners
//             of the QR code in the image using the finder pattern for that.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersFromFinderPattern.cpp
 *
 * @brief Defines functor class for retrieving the perspective corners
 * of the QR code in the image using the finder pattern for that.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "PerspCornersFromFinderPattern.h"
#include "PerspCornersHelper.h"
#include "../../../common/Line2D.h"

namespace barcodes {

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
 *  D point is retrieved from the intersection of the CN line and AM line
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
void PerspCornersFromFinderPattern::getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const {

	//============= DETERMINING THE THREE CORNERS (A, B, C) FROM THE DETECTED MARKS

	corners.clear();
	if (!PerspCornersHelper::sortDetectedMarks(detectedMarks)) {
		return;
	}

	//============= DETERMINING THE FROUTH UNKNOWN CORNER (D) BY RECT SAMPLING

	// The start point from which sampling begins, its distance is equal to distance
	Point2f pivotPoint = detectedMarks[0].points[0];
	Vector2Df lineVector(detectedMarks[0].points[0], detectedMarks[0].points[3]);

	// Now find the second edge
	Point2f pivotPoint2 = detectedMarks[2].points[0];
	Vector2Df lineVector2(detectedMarks[2].points[0], detectedMarks[2].points[1]);

	// Finally finding the 4th corner from the intersection of the CN and AM lines
	Point fourthCorner;
	if (!Line2D::intersection(Line2D(pivotPoint, lineVector + pivotPoint),
			Line2D(pivotPoint2, lineVector2 + pivotPoint2), fourthCorner)) {
		return;
	}

	// The outer point we will push as a result perspective points
	corners.push_back(detectedMarks[2].points[0]);
	corners.push_back(fourthCorner);
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);
}

} /* namespace barcodes */

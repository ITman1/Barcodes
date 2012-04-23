///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersFromFinderPattern.h
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines functor class for retrieving the perspective corners
//             of the QR code in the image using the alignment pattern for that.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersFromFinderPattern.h
 *
 * @brief Defines functor class for retrieving the perspective corners
 * of the QR code in the image using the alignment pattern for that.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef PERSPCORNERSFROMALIGNMENTPATTERN_H_
#define PERSPCORNERSFROMALIGNMENTPATTERN_H_

#include "GetPerspCorners.h"

namespace barcodes {
using namespace cv;

/**
 * Functor class which serves for retrieving the perspective corners of
 * the QR code in the image. For getting the fourth corner it especially uses
 * diagonal line constructed through the most right bottom alignment mark.
 */
class PerspCornersFromAlignmentPattern:public GetPerspCorners {
public:
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
	void getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const;
};

} /* namespace barcodes */
#endif /* PERSPCORNERSFROMALIGNMENTPATTERN_H_ */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       GetPerspCorners.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines base class for functors which retrieves corners of the
//             QR code from the perspective projection.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file GetPerspCorners.h
 *
 * @brief Defines base class for functors which retrieves corners of the QR
 * code from the perspective projection.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef GETPERSPCORNERS_H_
#define GETPERSPCORNERS_H_

#include <opencv2/core/core.hpp>
#include "../../DataSegments.h"
#include "../../DetectedMarks.h"
#include "../../../common/Vector2D.h"

namespace barcodes {
using namespace cv;

/**
 * Base class of the functor for getting the localization perspective corners from the QR code.
 */
class GetPerspCorners {
public:
	virtual ~GetPerspCorners() {}
	/**
	 * Retrieves four corners A,B,C,D for perspective transformation and sorts the
	 * detected marks and theirs points.
	 *
	 * Retrieved corners are sorted C,D,A,B respectively to be
	 * applicable for the perspective transformation.
	 *
	 * \code
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
	virtual void getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const {
		corners.clear();
	}
};

} /* namespace barcodes */
#endif /* GETPERSPCORNERS_H_ */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersHelper.h
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines helper class which gathers some common methods
//             for getting the perspective corners.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersHelper.h
 *
 * @brief Defines helper class which gathers some common methods
 * for getting the perspective corners.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef PERSPCORNERSHELPER_H_
#define PERSPCORNERSHELPER_H_

#include "../../DataSegments.h"
#include "../../DetectedMarks.h"

namespace barcodes {
using namespace cv;

/**
 * Helper class which gathers some common methods for getting the perspective corners.
 */
class PerspCornersHelper {
public:
	static bool sortDetectedMarks(DetectedMarks &detectedMarks);
};

} /* namespace barcodes */
#endif /* PERSPCORNERSHELPER_H_ */

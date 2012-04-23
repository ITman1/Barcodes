///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrBuildHelper.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrBuildHelper class which groups helper static methods
//             for building the parts of the QR code.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBuildHelper.h
 *
 * @brief Defines QrBuildHelper class which groups helper static methods
 *        for building the parts of the QR code.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRBUILDHELPER_H_
#define QRBUILDHELPER_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

/**
 * Helper class which groups static methods for building the parts of the QR codes.
 */
class QrBuildHelper {
public:
	/**
	 * Builds the finder pattern with the specified size.
	 *
	 * @param size Size of the finder mark.
	 * @return Image with the built finder mark.
	 */
	static Mat buildQrMark(int size);

	/**
	 * Builds the alignment pattern with the specified size.
	 *
	 * @param size Size of the alignment mark.
	 * @return Image with the built alignment mark.
	 */
	static Mat buildAlignementMark(int size);
};

} /* namespace barcodes */
#endif /* QRBUILDHELPER_H_ */

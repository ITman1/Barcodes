///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrBarcode.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrBarcode base class which should be used for
//             the creation of specific QR code classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBarcode.cpp
 *
 * @brief Defines members of QrBarcode base class which should be used for
 *        the creation of specific QR code classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrBarcode.h"

#define DEBUG_TAG "QrBarcode.cpp"

namespace barcodes {

/**
 * Detects QR code and returns localization marks.
 *
 * @param image Image with the QR code.
 * @param detectedMarks Recognized localization marks.
 * @param flags Detection flags.
 */
void QrBarcode::detect(Image &image, DetectedMarks &detectedMarks, int flags) {
	QrDetector::getInstance()->detect(image, detectedMarks, flags);
}

/**
 * Decodes QR code on the image and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param dataSegments Result decoded data segments.
 * @param flags Flags used for detection and decoding.
 */
void QrBarcode::decode(Image &image, DataSegments &dataSegments, int flags) {
	QrDecoder::getInstance()->decode(image, dataSegments, flags);
}

}




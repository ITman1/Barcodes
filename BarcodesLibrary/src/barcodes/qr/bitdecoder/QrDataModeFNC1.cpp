///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeFNC1.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDataModeFNC1 class for decoding
//             the FNC1 mode in first position.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeFNC1.cpp
 *
 * @brief Defines members of QrDataModeFNC1 class for decoding
 *        the FNC1 mode in first position.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeFNC1.h"

namespace barcodes {

/**
 * Decodes one data segment. No data are appended.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeFNC1::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See 6.4.8.1 FNC1 in first position (ISO 18004:2006)
}

} /* namespace barcodes */

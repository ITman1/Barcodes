///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeFNC12.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDataModeFNC12 class for decoding
//             the FNC1 mode in second position.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeFNC12.cpp
 *
 * @brief Defines members of QrDataModeFNC12 class for decoding
 *        the FNC1 mode in second position.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeFNC12.h"

namespace barcodes {

/**
 * Decodes one data segment. No data contains only application indicator.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeFNC1_2::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See 6.4.8.2 FNC1 in second position (ISO 18004:2006)

	uint32_t byte;
	bitStream(8) >> byte;

	dataSegment.data.push_back(byte);
}

} /* namespace barcodes */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeStructuredAppend.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines membes of QrDataModeStructuredAppend class for decoding
//             the Structured append mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeStructuredAppend.cpp
 *
 * @brief Defines members of QrDataModeStructuredAppend class for decoding
 *        the Structured append mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeStructuredAppend.h"

namespace barcodes {

/**
 * Decodes one data segment. Symbol sequence indicator is stored in the first byte.
 * Parity data is stored in the second byte.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeStructuredAppend::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	uint8_t byte;

	bitStream(8) >> byte;
	dataSegment.data.push_back(byte);

	bitStream(8) >> byte;
	dataSegment.data.push_back(byte);
}

} /* namespace barcodes */

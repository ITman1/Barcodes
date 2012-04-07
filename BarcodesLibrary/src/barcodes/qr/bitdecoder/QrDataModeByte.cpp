///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeByte.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDataModeByte class for decoding
//             the byte mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeByte.cpp
 *
 * @brief Defines members of QrDataModeByte class for decoding
 *        the byte mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeByte.h"

namespace barcodes {

/**
 * Decodes one data segment. Decoded data are left unchanged.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeByte::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See Table 3 — Number of bits in character count indicator for QR Code 2005 (ISO 18004:2006)

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 8;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 16;
	}

	// See 6.4.5 Byte mode (ISO 18004:2006)

	uint32_t length;
	bitStream(characterCountBits) >> length;

	size_t bytes = 0;
	uint8_t byte;
	while ((!bitStream.isEnd()) && (bytes < length)) {
		bytes++;
		bitStream(8) >> byte;
		dataSegment.data.push_back(byte);
	}
}

} /* namespace barcodes */

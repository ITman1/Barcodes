///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeKanji.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the QrDataModeKanji class for decoding
//             the Kanji characters into Shift JIS system.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeKanji.cpp
 *
 * @brief Defines members of the QrDataModeKanji class for decoding
 *        the Kanji characters into Shift JIS system.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeKanji.h"

namespace barcodes {

/**
 * Decodes one data segment. Encoded Kanji characters are converted into Shift JIS encoding.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeKanji::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See Table 3 — Number of bits in character count indicator for QR Code 2005 (ISO 18004:2006)

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 8;
	} else if (versionInformation < QrVersionInformation::VERSION_27) {
		characterCountBits = 10;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 12;
	}

	// See 6.4.6 Kanji mode (ISO 18004:2006)

	uint32_t length;
	bitStream(characterCountBits) >> length;

	int readKanji;
	size_t leftKanji = length;
	while ((!bitStream.isEnd()) && (leftKanji > 0)) {
		bitStream(13) >> readKanji;

		// Converts encoded characters in Kanji mode into Shift JIS encoding
		int processed = (readKanji % 0x0C0) | ((readKanji / 0x0C0) << 8);
		if (processed < 0x01F00) { // 0x8140 - 0x9FFC
			processed += 0x08140;
		} else { // 0xE040 - 0xEBBF
			processed += 0x0C140;
		}

		dataSegment.data.push_back(processed >> 8);
		dataSegment.data.push_back(processed);

		leftKanji--;
	}
}

} /* namespace barcodes */

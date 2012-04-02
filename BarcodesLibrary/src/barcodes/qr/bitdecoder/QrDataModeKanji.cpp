/*
 * QrDataModeKanji.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeKanji.h"

namespace barcodes {

void QrDataModeKanji::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 8;
	} else if (versionInformation < QrVersionInformation::VERSION_27) {
		characterCountBits = 10;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 12;
	}

	uint32_t length;
	bitStream(characterCountBits) >> length;

	int readKanji;
	size_t leftKanji = length;
	while ((!bitStream.isEnd()) && (leftKanji > 0)) {
		bitStream(13) >> readKanji;

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

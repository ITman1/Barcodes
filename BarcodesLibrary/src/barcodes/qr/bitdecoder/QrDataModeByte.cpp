/*
 * QrDataModeByte.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeByte.h"

namespace barcodes {

void QrDataModeByte::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 8;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 16;
	}

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

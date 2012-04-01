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
	dataSegment.mode = mode;
	dataSegment.data.clear();

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 8;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 16;
	}

	DataBitsStream::TYPE length;
	bitStream(characterCountBits) >> length;

	size_t bytes = 0;
	DataBitsStream::TYPE byte;
	while ((!bitStream.isEnd()) && (bytes < length)) {
		bytes++;
		bitStream(8) >> byte;
		dataSegment.data.push_back(byte);
	}

	dataSegment.remainderBits = 8 - bitStream.lastReadBits();
}

} /* namespace barcodes */

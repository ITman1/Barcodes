/*
 * QrDataModeFNC12.cpp
 *
 *  Created on: 2.4.2012
 *      Author: Scotty
 */

#include "QrDataModeFNC12.h"

namespace barcodes {

void QrDataModeFNC1_2::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	uint32_t byte;
	bitStream(8) >> byte;

	dataSegment.data.push_back(byte);
}

} /* namespace barcodes */

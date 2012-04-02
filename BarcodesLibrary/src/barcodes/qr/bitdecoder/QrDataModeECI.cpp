/*
 * QrDataModeECI.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeECI.h"

namespace barcodes {

void QrDataModeECI::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	uint8_t byte;
	int assignmentValue1 = -1, assignmentValue2 = -1, assignmentValue3 = -1;

	bitStream(8) >> byte;

	if (byte & 0x80) {
		if (byte & 0x40) {
			assignmentValue1 = byte & 0x3F;
			bitStream(8) >> byte;
			assignmentValue2 = byte;
		} else {
			assignmentValue1 = byte & 0x10;
		}

		bitStream(8) >> byte;
		assignmentValue3 = byte;
	} else {
		assignmentValue1 = byte & 0x7F;
	}

	if (assignmentValue1 != -1) dataSegment.data.push_back(assignmentValue1);
	if (assignmentValue2 != -1) dataSegment.data.push_back(assignmentValue2);
	if (assignmentValue3 != -1) dataSegment.data.push_back(assignmentValue3);
}

} /* namespace barcodes */

/*
 * QrDataModeStructuredAppend.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeStructuredAppend.h"

namespace barcodes {

void QrDataModeStructuredAppend::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();
}

} /* namespace barcodes */

/*
 * QrDataModeFNC1.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeFNC1.h"

namespace barcodes {

void QrDataModeFNC1::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();
}

} /* namespace barcodes */

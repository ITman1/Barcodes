/*
 * QrDataModeFNC1.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEFNC1_H_
#define QRDATAMODEFNC1_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeFNC1: public QrDataMode {
public:
	QrDataModeFNC1(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEFNC1_H_ */

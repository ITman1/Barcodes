/*
 * QrDataModeFNC12.h
 *
 *  Created on: 2.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEFNC12_H_
#define QRDATAMODEFNC12_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeFNC1_2: public QrDataMode {
public:
	QrDataModeFNC1_2(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};


} /* namespace barcodes */
#endif /* QRDATAMODEFNC12_H_ */

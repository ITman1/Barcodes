/*
 * QrDataModeByte.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEBYTE_H_
#define QRDATAMODEBYTE_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeByte: public QrDataMode {
public:
	QrDataModeByte(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEBYTE_H_ */

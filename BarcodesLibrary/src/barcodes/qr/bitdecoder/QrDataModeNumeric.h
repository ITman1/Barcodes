/*
 * QrDataModeNumeric.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODENUMERIC_H_
#define QRDATAMODENUMERIC_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeNumeric: public QrDataMode {
public:
	QrDataModeNumeric(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODENUMERIC_H_ */

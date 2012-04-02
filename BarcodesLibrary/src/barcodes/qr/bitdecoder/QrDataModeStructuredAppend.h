/*
 * QrDataModeStructuredAppend.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODESTRUCTUREDAPPEND_H_
#define QRDATAMODESTRUCTUREDAPPEND_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeStructuredAppend: public QrDataMode {
public:
	QrDataModeStructuredAppend(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODESTRUCTUREDAPPEND_H_ */

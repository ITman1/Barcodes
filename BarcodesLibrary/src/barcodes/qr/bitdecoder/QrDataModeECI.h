/*
 * QrDataModeECI.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEECI_H_
#define QRDATAMODEECI_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeECI: public QrDataMode {
public:
	QrDataModeECI(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEECI_H_ */

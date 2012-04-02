/*
 * QrDataModeAlphaNumeric.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEALPHANUMERIC_H_
#define QRDATAMODEALPHANUMERIC_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeAlphaNumeric: public QrDataMode {
protected:
	static const size_t ALPHANUMERIC_MAP_TABLE_SIZE = 45;
	static const char ALPHANUMERIC_MAP_TABLE[ALPHANUMERIC_MAP_TABLE_SIZE];
public:
	QrDataModeAlphaNumeric(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEALPHANUMERIC_H_ */

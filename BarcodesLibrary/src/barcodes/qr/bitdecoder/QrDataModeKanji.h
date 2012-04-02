/*
 * QrDataModeKanji.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODEKANJI_H_
#define QRDATAMODEKANJI_H_

#include "QrDataMode.h"

namespace barcodes {

class QrDataModeKanji: public QrDataMode {
public:
	QrDataModeKanji(int mode) : QrDataMode(mode) {}

	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEKANJI_H_ */

/*
 * QrBitDecoder.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRBITDECODER_H_
#define QRBITDECODER_H_

#include "QrDataModeByte.h"

namespace barcodes {

class QrBitDecoder {
private:
	static const int DATA_MODE_TERMINATOR = 0;
	static const QrBitDecoder BIT_DECODER_INSTANCE;
	static const map<uint32_t, QrDataMode *> MODE_MAP;

	QrBitDecoder() {}
public:
	virtual ~QrBitDecoder() {}

	static QrBitDecoder getInstance();

	void decode(BitArray &bitArray, vector<DataSegment> &dataSegments, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRBITDECODER_H_ */

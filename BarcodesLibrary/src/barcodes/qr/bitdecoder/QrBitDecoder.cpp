/*
 * QrBitDecoder.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrBitDecoder.h"

namespace barcodes {

const QrBitDecoder QrBitDecoder::BIT_DECODER_INSTANCE;


QrDataModeByte BYTE_MODE;
pair<uint32_t, QrDataMode *> QrBitDecoder_mode_mapping[] = {
	make_pair(BYTE_MODE.getMode(), &BYTE_MODE)
};

const map<uint32_t, QrDataMode *> QrBitDecoder::MODE_MAP(
		QrBitDecoder_mode_mapping,	QrBitDecoder_mode_mapping
		+ sizeof QrBitDecoder_mode_mapping / sizeof QrBitDecoder_mode_mapping[0]);

QrBitDecoder QrBitDecoder::getInstance() {
	return BIT_DECODER_INSTANCE;
}

void QrBitDecoder::decode(BitArray &bitArray, vector<DataSegment> &dataSegments, const QrVersionInformation &versionInformation) {
	dataSegments.clear();

	DataBitsStream bitStream(bitArray);
	DataSegment dataSegment;

	int modeBitCount = 1;
	if (versionInformation <= QrVersionInformation::VERSION_40) {
		modeBitCount = 4;
	}

	uint32_t mode;
	while (!bitStream.isEnd()) {
		bitStream(modeBitCount) >> mode;
		if ((mode == DATA_MODE_TERMINATOR) || MODE_MAP.find(mode) == MODE_MAP.end()) {
			break;
		}

		QrDataMode *dataModeDecoder = MODE_MAP.at(mode);

		dataModeDecoder->decode(bitStream, dataSegment, versionInformation);
		dataSegments.push_back(dataSegment);
	}
}

} /* namespace barcodes */

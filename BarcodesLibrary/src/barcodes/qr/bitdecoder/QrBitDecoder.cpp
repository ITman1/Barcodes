/*
 * QrBitDecoder.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeAlphaNumeric.h"
#include "QrDataModeNumeric.h"
#include "QrDataModeECI.h"
#include "QrDataModeFNC1.h"
#include "QrDataModeKanji.h"
#include "QrDataModeStructuredAppend.h"
#include "QrDataModeFNC12.h"

#include "QrBitDecoder.h"

namespace barcodes {

const QrBitDecoder QrBitDecoder::BIT_DECODER_INSTANCE;


QrDataModeNumeric          NUMERIC_MODE         (0x01);
QrDataModeAlphaNumeric     ALPHANUMERIC_MODE    (0x02);
QrDataModeStructuredAppend STRUCTUREDAPPEND_MODE(0x03);
QrDataModeByte             BYTE_MODE            (0x04);
QrDataModeFNC1             FNC1_MODE            (0x05);
QrDataModeECI              ECI_MODE             (0x07);
QrDataModeKanji            KANJI_MODE           (0x08);
QrDataModeFNC1_2           FNC1_2_MODE          (0x09);
pair<uint32_t, QrDataMode *> QrBitDecoder_mode_mapping[] = {
	make_pair(BYTE_MODE.getMode(),             &BYTE_MODE),
	make_pair(ALPHANUMERIC_MODE.getMode(),     &ALPHANUMERIC_MODE),
	make_pair(NUMERIC_MODE.getMode(),          &NUMERIC_MODE),
	make_pair(ECI_MODE.getMode(),              &ECI_MODE),
	make_pair(FNC1_MODE.getMode(),             &FNC1_MODE),
	make_pair(STRUCTUREDAPPEND_MODE.getMode(), &STRUCTUREDAPPEND_MODE),
	make_pair(KANJI_MODE.getMode(),            &KANJI_MODE),
	make_pair(FNC1_2_MODE.getMode(),           &FNC1_2_MODE)
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

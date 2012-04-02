/*
 * QrDataModeAlphaNumeric.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "QrDataModeAlphaNumeric.h"

namespace barcodes {

const char QrDataModeAlphaNumeric::ALPHANUMERIC_MAP_TABLE[ALPHANUMERIC_MAP_TABLE_SIZE] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
	'W', 'X', 'Y', 'Z', ' ', '$', '%', '*',
	'+', '-', '.', '/', ':'
};

void QrDataModeAlphaNumeric::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 9;
	} else if (versionInformation < QrVersionInformation::VERSION_27) {
		characterCountBits = 11;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 13;
	}

	uint32_t length;
	bitStream(characterCountBits) >> length;

	uint32_t readSegment;
	size_t leftChars = length;
	size_t index1, index2;
	char char1, char2;
	while ((!bitStream.isEnd()) && (leftChars > 0)) {
		if (leftChars == 1) {
			bitStream(6) >> readSegment;

			char1 = (readSegment < ALPHANUMERIC_MAP_TABLE_SIZE)? ALPHANUMERIC_MAP_TABLE[readSegment] : '~';

			leftChars -= 1;
			dataSegment.data.push_back(char1);
		} else {
			bitStream(11) >> readSegment;
			index2 = readSegment % 45;
			index1 = (readSegment - index2) / 45;

			char1 = (index1 < ALPHANUMERIC_MAP_TABLE_SIZE)? ALPHANUMERIC_MAP_TABLE[index1] : '~';
			char2 = (index2 < ALPHANUMERIC_MAP_TABLE_SIZE)? ALPHANUMERIC_MAP_TABLE[index2] : '~';

			dataSegment.data.push_back(char1);
			dataSegment.data.push_back(char2);
			leftChars -= 2;
		}


	}

}

} /* namespace barcodes */

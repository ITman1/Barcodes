/*
 * QrDataModeNumeric.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include <iomanip>
#include <sstream>
#include <iostream>

#include "QrDataModeNumeric.h"

namespace barcodes {
using namespace std;

void QrDataModeNumeric::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 10;
	} else if (versionInformation < QrVersionInformation::VERSION_27) {
		characterCountBits = 12;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 14;
	}

	uint32_t length;
	bitStream(characterCountBits) >> length;

	uint32_t readSegment;
	size_t leftNumbers = length;
	size_t readNumbers = 0;
	while ((!bitStream.isEnd()) && (leftNumbers > 0)) {
		if (leftNumbers < 3) {
			bitStream(4 + 3 * ((leftNumbers + 1) % 2)) >> readSegment;
			readNumbers = leftNumbers % 3;
		} else {
			bitStream(10) >> readSegment;
			readNumbers = 3;
		}

		stringstream ss;
		ss << setw(readNumbers) << right << readSegment;
		string number = ss.str();
		replace(number.begin(), number.end(), ' ', '0');

		dataSegment.data.insert(dataSegment.data.end(), number.begin(), number.begin() + readNumbers);

		leftNumbers -= readNumbers;
	}
}

} /* namespace barcodes */

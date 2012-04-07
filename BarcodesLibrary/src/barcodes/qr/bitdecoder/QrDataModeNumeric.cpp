///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeNumeric.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDataModeNumeric class for decoding
//             the numeric mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeNumeric.h
 *
 * @brief Defines members of QrDataModeNumeric class for decoding
 *        the numeric mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iomanip>
#include <sstream>
#include <iostream>

#include "QrDataModeNumeric.h"

namespace barcodes {
using namespace std;

/**
 * Decodes one data segment. Decoded number are stored in the result data as ASCI characters.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeNumeric::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See Table 3 — Number of bits in character count indicator for QR Code 2005 (ISO 18004:2006)

	int characterCountBits = 0;
	if (versionInformation < QrVersionInformation::VERSION_10) {
		characterCountBits = 10;
	} else if (versionInformation < QrVersionInformation::VERSION_27) {
		characterCountBits = 12;
	} else if (versionInformation <= QrVersionInformation::VERSION_40) {
		characterCountBits = 14;
	}

	// See 6.4.3 Numeric mode (ISO 18004:2006)

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

		// Converts number into string  the fills with zeros if necessary

		stringstream ss;
		ss << setw(readNumbers) << right << readSegment;
		string number = ss.str();
		replace(number.begin(), number.end(), ' ', '0');

		dataSegment.data.insert(dataSegment.data.end(), number.begin(), number.begin() + readNumbers);

		leftNumbers -= readNumbers;
	}
}

} /* namespace barcodes */

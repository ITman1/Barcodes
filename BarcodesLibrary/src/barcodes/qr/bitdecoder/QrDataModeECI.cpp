///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeECI.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDataModeECI class for decoding
//             the ECI mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeECI.h
 *
 * @brief Defines members of QrDataModeECI class for decoding
 *        the ECI mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "QrDataModeECI.h"

namespace barcodes {

/**
 * Decodes one data segment. Decoded ECI Assignment Value is stored into data without prefix bits.
 *
 * @param bitStream Bit stream of data.
 * @param dataSegment Result segment of data.
 * @param versionInformation Version of the QR code which data are being decoded.
 */
void QrDataModeECI::decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) {
	dataSegment.flags = 0;
	dataSegment.remainderBits = 0;
	dataSegment.mode = mode;
	dataSegment.data.clear();

	// See 6.4.2.1 ECI Designator (ISO 18004:2006)

	uint8_t byte;
	int assignmentValue1 = -1, assignmentValue2 = -1, assignmentValue3 = -1;

	bitStream(8) >> byte;

	if (byte & 0x80) {
		if (byte & 0x40) {
			assignmentValue1 = byte & 0x3F;
			bitStream(8) >> byte;
			assignmentValue2 = byte;
		} else {
			assignmentValue1 = byte & 0x10;
		}

		bitStream(8) >> byte;
		assignmentValue3 = byte;
	} else {
		assignmentValue1 = byte & 0x7F;
	}

	if (assignmentValue1 != -1) dataSegment.data.push_back(assignmentValue1);
	if (assignmentValue2 != -1) dataSegment.data.push_back(assignmentValue2);
	if (assignmentValue3 != -1) dataSegment.data.push_back(assignmentValue3);
}

} /* namespace barcodes */

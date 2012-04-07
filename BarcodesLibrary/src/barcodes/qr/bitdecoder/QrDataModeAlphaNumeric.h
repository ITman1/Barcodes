///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeAlphaNumeric.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeAlphaNumeric class for decoding
//             the alphanumeric mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeAlphaNumeric.h
 *
 * @brief Defines QrDataModeAlphaNumeric class for decoding
 *        the alphanumeric mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEALPHANUMERIC_H_
#define QRDATAMODEALPHANUMERIC_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for alphanumeric mode.
 * Data are decoded as ASCI characters.
 */
class QrDataModeAlphaNumeric: public QrDataMode {
protected:
	/**
	 * Size of the table which maps encoded alphanumeric characters to ASCI characters.
	 */
	static const size_t ALPHANUMERIC_MAP_TABLE_SIZE = 45;

	/**
	 * Table which maps encoded alphanumeric characters to ASCI characters.
	 */
	static const char ALPHANUMERIC_MAP_TABLE[ALPHANUMERIC_MAP_TABLE_SIZE];
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeAlphaNumeric(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Data are decoded as ASCI characters.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEALPHANUMERIC_H_ */

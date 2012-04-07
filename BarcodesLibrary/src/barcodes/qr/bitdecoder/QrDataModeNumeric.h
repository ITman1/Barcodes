///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeNumeric.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeNumeric class for decoding
//             the numeric mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeNumeric.h
 *
 * @brief Defines QrDataModeNumeric class for decoding
 *        the numeric mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODENUMERIC_H_
#define QRDATAMODENUMERIC_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for numeric mode.
 */
class QrDataModeNumeric: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeNumeric(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Decoded number are stored in the result data as ASCI characters.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODENUMERIC_H_ */

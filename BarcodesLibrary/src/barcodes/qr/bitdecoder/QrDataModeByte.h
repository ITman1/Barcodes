///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeByte.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeByte class for decoding
//             the byte mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeByte.h
 *
 * @brief Defines QrDataModeByte class for decoding
 *        the byte mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEBYTE_H_
#define QRDATAMODEBYTE_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for byte mode.
 */
class QrDataModeByte: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeByte(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Decoded data are left unchanged.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEBYTE_H_ */

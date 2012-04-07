///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeStructuredAppend.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeStructuredAppend class for decoding
//             the Structured append mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeStructuredAppend.h
 *
 * @brief Defines QrDataModeStructuredAppend class for decoding
 *        the Structured append mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODESTRUCTUREDAPPEND_H_
#define QRDATAMODESTRUCTUREDAPPEND_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for Structured append mode.
 */
class QrDataModeStructuredAppend: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeStructuredAppend(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Symbol sequence indicator is stored in the first byte.
	 * Parity data is stored in the second byte.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODESTRUCTUREDAPPEND_H_ */

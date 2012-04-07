///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeFNC1.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeFNC1 class for decoding
//             the FNC1 mode in first position.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeFNC1.h
 *
 * @brief Defines QrDataModeFNC1 class for decoding
 *        the FNC1 mode in first position.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEFNC1_H_
#define QRDATAMODEFNC1_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for FNC1 mode in first position.
 */
class QrDataModeFNC1: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeFNC1(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. No data are appended.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEFNC1_H_ */

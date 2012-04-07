///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeFNC12.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeFNC12 class for decoding
//             the FNC1 mode in second position.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeFNC12.h
 *
 * @brief Defines QrDataModeFNC12 class for decoding
 *        the FNC1 mode in second position.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEFNC12_H_
#define QRDATAMODEFNC12_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for FNC1 mode in second position.
 */
class QrDataModeFNC1_2: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeFNC1_2(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. No data contains only application indicator.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};


} /* namespace barcodes */
#endif /* QRDATAMODEFNC12_H_ */

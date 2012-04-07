///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeECI.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeECI class for decoding
//             the ECI mode.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeECI.h
 *
 * @brief Defines QrDataModeECI class for decoding
 *        the ECI mode.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEECI_H_
#define QRDATAMODEECI_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for ECI header.
 */
class QrDataModeECI: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeECI(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Decoded ECI Assignment Value is stored into data without prefix bits.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEECI_H_ */

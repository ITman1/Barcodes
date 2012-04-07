///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataModeKanji.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataModeKanji class for decoding
//             the Kanji characters into Shift JIS system.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDataModeKanji.h
 *
 * @brief Defines QrDataModeKanji class for decoding
 *        the Kanji characters into Shift JIS system.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODEKANJI_H_
#define QRDATAMODEKANJI_H_

#include "QrDataMode.h"

namespace barcodes {

/**
 * Class which implements decoder for Kanji mode.
 */
class QrDataModeKanji: public QrDataMode {
public:
	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataModeKanji(int mode) : QrDataMode(mode) {}

	/**
	 * Decodes one data segment. Encoded Kanji characters are converted into Shift JIS encoding.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRDATAMODEKANJI_H_ */

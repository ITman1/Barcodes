///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrBitDecoder.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrBitDecoder class which decodes the decoded bit array
//             of the QR code and returns decoded data segments.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBitDecoder.h
 *
 * @brief Defines QrBitDecoder class which decodes the decoded bit array
 *        of the QR  code and returns decoded data segments.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRBITDECODER_H_
#define QRBITDECODER_H_

#include "QrDataModeByte.h"

namespace barcodes {

/**
 * Class decodes the bit array. For decoding uses decoders which derives
 * QrDataMode class.
 */
class QrBitDecoder {
private:
	/**
	 * Mode identifier for the end of the data stream.
	 */
	static const uint32_t DATA_MODE_TERMINATOR = 0;

	/**
	 * Instance of the bit decoder.
	 */
	static const QrBitDecoder BIT_DECODER_INSTANCE;

	/**
	 * Maps mode identifiers to proper mode decoders.
	 */
	static const map<uint32_t, QrDataMode *> MODE_MAP;

	QrBitDecoder() {}
public:
	virtual ~QrBitDecoder() {}

	/**
	 * Returns instance of the bit decoder.
	 *
	 * @return instance of the bit decoder.
	 */
	static QrBitDecoder getInstance();

	/**
	 * Decodes bit array and returns data segments.
	 *
	 * @param bitArray Bit array to be decoded.
	 * @param dataSegments Result data segments.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	void decode(BitArray &bitArray, vector<DataSegment> &dataSegments, const QrVersionInformation &versionInformation);
};

} /* namespace barcodes */
#endif /* QRBITDECODER_H_ */

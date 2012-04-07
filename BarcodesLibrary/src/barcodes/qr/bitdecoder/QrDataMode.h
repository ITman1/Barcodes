///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDataMode.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDataMode base class for all data mode decoders.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBitDecoder.h
 *
 * @brief Defines QrDataMode base class for all data mode decoders.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDATAMODE_H_
#define QRDATAMODE_H_

#include "../../DataSegments.h"
#include "../QrVersionInformation.h"
#include "../../common/BitStream.h"

namespace barcodes {

typedef BitStreamReverseCodeword<uint32_t> DataBitsStream;

/**
 * Base class for all data mode decoders.
 */
class QrDataMode {
protected:
	int mode;	/**< Mode number of the decoder */
public:
	/**
	 * Mode number of the invalid data decoder.
	 */
	const static int INVALID_DATA_MODE = -1;

	/**
	 * Constructs data mode decoder with the specified mode number.
	 *
	 * @param mode Mode of the decoder.
	 */
	QrDataMode(int mode) : mode(mode) {}

	/**
	 * Abstract method for decoding one data segment.
	 *
	 * @param bitStream Bit stream of data.
	 * @param dataSegment Result segment of data.
	 * @param versionInformation Version of the QR code which data are being decoded.
	 */
	virtual void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) = 0;
	virtual ~QrDataMode() {}

	/**
	 * Returns mode number of this data decoder.
	 *
	 * @return Mode number of this data decoder.
	 */
	inline int getMode() const {
		return mode;
	}

	inline bool operator!=(const QrDataMode &rhs) const {
		return !(*this == rhs);
	}

	inline bool operator==(const QrDataMode &rhs) const {
		return mode == rhs.mode;
	}
};

} /* namespace barcodes */
#endif /* QRDATAMODE_H_ */

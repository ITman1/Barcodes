/*
 * QrDataMode.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRDATAMODE_H_
#define QRDATAMODE_H_

#include "../../DataSegments.h"
#include "../QrVersionInformation.h"
#include "../../common/BitStream.h"

namespace barcodes {

typedef BitStreamReverseCodeword<uint32_t> DataBitsStream;

class QrDataMode {
protected:
	int mode;
public:
	const static int INVALID_DATA_MODE = -1;

	QrDataMode(int mode) : mode(mode) {}

	virtual void decode(DataBitsStream &bitStream, DataSegment &dataSegment, const QrVersionInformation &versionInformation) = 0;
	virtual ~QrDataMode() {}

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

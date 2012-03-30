/*
 * Decoder.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef DECODER_H_
#define DECODER_H_

#include "../common.h"

namespace barcodes {

class Decoder {
protected:
	Decoder() {}
	virtual ~Decoder() {}
public:
	virtual void decode(Image &image, ByteArray &data, int flags = 0) const = 0;
	virtual const Decoder *getDecoder() const = 0;
};

} /* namespace barcodes */
#endif /* DECODER_H_ */

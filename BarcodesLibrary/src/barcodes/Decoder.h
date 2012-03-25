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
public:
	Decoder() {}
	virtual ~Decoder() {}

	virtual void decode(Image &image, ByteArray &data, int flags = 0);
};

} /* namespace barcodes */
#endif /* DECODER_H_ */

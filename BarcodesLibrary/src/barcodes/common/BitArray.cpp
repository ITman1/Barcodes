/*
 * BitArray.cpp
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#include "BitArray.h"
#include "BitStream.h"

namespace barcodes {

BitArray::BitArray() {}

BitArray::BitArray(uint64_t value) {
	uint64_t mask = 1;

	for (int i = 0; i < 64; i++) {
		pushBit(mask & value);
		mask <<= 1;
	}
}

uint64_t BitArray::toULong() {
	BitStream <uint64_t> bitsStream(*this);
	uint64_t number = 0;
	bitsStream >> number;
	return number;
}

} /* namespace barcodes */

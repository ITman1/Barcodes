/*
 * BitArray.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <opencv2/core/core.hpp>

#include "../../common.h"

namespace barcodes {
using namespace cv;

class BitArray: public ByteArray {
public:
	BitArray() {}
	BitArray(uint64_t value) {
		uint64_t mask = 1;

		for (int i = 0; i < 64; i++) {
			pushBit(mask & value);
			mask <<= 1;
		}
	}

	virtual ~BitArray() {}

	inline bool getBit(int index) {
		return at(index);
	}

	inline void setBit(int index, bool bit) {
		at(index) = bit;
	}

	inline void pushBit(bool bit) {
		push_back(bit);
	}

	inline void push(BitArray &arr) {
		insert(end(), arr.begin(), arr.end());
	}

	inline uint64_t toULong() {
		uint64_t ret = 0;

		for (uint32_t i = 0; (i < 64) && (i < size()); i++) {
			ret += ((uint64_t)getBit(i)) << i;
		}

		return ret;
	}
};

} /* namespace barcodes */
#endif /* BITARRAY_H_ */

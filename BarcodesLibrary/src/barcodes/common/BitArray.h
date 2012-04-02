/*
 * BitArray.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

class BitArray: public vector<bool> {
public:
	BitArray();
	BitArray(uint64_t value);

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

	uint64_t toULong();
};

} /* namespace barcodes */

namespace barcodes {

class _BitArray {
public:
	_BitArray();
	virtual ~_BitArray();
};

} /* namespace barcodes */
#endif /* BITARRAY_H_ */


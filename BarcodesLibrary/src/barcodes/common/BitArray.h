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

	template<typename T>
	inline void pushNumberReverse(T number, int bits = 8 * sizeof(T)) {
		for (int i = 0; i < bits; i++, number >>= 1) {
			pushBit(number & 0x01);
		}
	}

	template<typename T>
	inline void pushNumber(T number, int bits = 8 * sizeof(T)) {
		for (int i = bits - 1; i >= 0; i--) {
			pushBit(number & (T)(0x01 << i));
		}
	}

	uint64_t toULong();
};

} /* namespace barcodes */

#endif /* BITARRAY_H_ */


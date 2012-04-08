///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       BitArray.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines BitArray class which holds bits inside vector of booleans
//             and implements some other additional methods above it.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file BitArray.h
 *
 * @brief Defines BitArray class which holds bits inside vector of booleans
 *        and implements some other additional methods above it.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

/**
 * Class which extends vector of booleans and implements additional methods
 * above it.
 */
class BitArray: public vector<bool> {
public:
	BitArray() {}

	/**
	 * Constructs array and fills first bits in the vector by passed value.
	 * Number of filled bits depends on the type of value.
	 *
	 * @param value Number by which will be filled bit array.
	 */
	template<typename T>
	BitArray(T value) {
		T mask = 1;

		for (int i = 0; i < 64; i++) {
			pushBit(mask & value);
			mask <<= 1;
		}
	}

	virtual ~BitArray() {}

	/**
	 * Returns bit on the specified position.
	 *
	 * @param index Index of the bit.
	 * @return Bit on the specified position.
	 */
	inline bool getBit(int index) {
		return at(index);
	}

	/**
	 * Sets bit on the specified position in the array.
	 *
	 * @param index Index where to set bit.
	 * @param bit Bit to set with this position.
	 */
	inline void setBit(int index, bool bit) {
		at(index) = bit;
	}

	/**
	 * Adds bit into array.
	 *
	 * @param bit Bit for adding at the end of the array.
	 */
	inline void pushBit(bool bit) {
		push_back(bit);
	}

	/**
	 * Adds whole array at the end of this array.
	 *
	 * @param arr Array of bits to be added at the end of this array.
	 */
	inline void push(BitArray &arr) {
		insert(end(), arr.begin(), arr.end());
	}

	/**
	 * Pushes bits of the number at the end of this array.
	 * Low significant bits are pushed first.
	 *
	 * @param number Number of which bits should be pushed into this array.
	 * @param bits Number of bits to be pushed into this array.
	 *        Default value is the size of the type of the number.
	 */
	template<typename T>
	inline void pushNumberReverse(T number, int bits = 8 * sizeof(T)) {
		for (int i = 0; i < bits; i++, number >>= 1) {
			pushBit(number & 0x01);
		}
	}

	/**
	 * Pushes bits of the number at the end of this array.
	 * Most significant bits are pushed first.
	 *
	 * @param number Number of which bits should be pushed into this array.
	 * @param bits Number of bits to be pushed into this array.
	 *        Default value is the size of the type of the number.
	 */
	template<typename T>
	inline void pushNumber(T number, int bits = 8 * sizeof(T)) {
		for (int i = bits - 1; i >= 0; i--) {
			pushBit(number & (T)(0x01 << i));
		}
	}

	/**
	 * Converts bits from the start of this array into number.
	 * First bits in the array represents low significant bits of the number.
	 *
	 * @param bits Number of bits to be converted into number.
	 *        Default value is the size of the type of the number.
	 */
	template<typename T>
	T toNumber(int bits = 8 * sizeof(T));
};

} /* namespace barcodes */

#endif /* BITARRAY_H_ */


///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       BitArray.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the BitArray class which holds bits inside
//             the vector of booleans.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file BitArray.cpp
 *
 * @brief Defines members of the BitArray class which holds bits inside
 *        the vector of booleans.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "BitStream.h"
#include "BitArray.h"

namespace barcodes {

/**
 * Converts bits from the start of this array into number.
 * First bits in the array represents low significant bits of the number.
 *
 * @param bits Number of bits to be converted into number.
 *        Default value is the size of the type of the number.
 */
template<typename T>
T BitArray::toNumber(int bits) {
	BitStream <T> bitsStream(*this);
	T number = 0;
	bitsStream(bits) >> number;
	return number;
}

template uint8_t BitArray::toNumber<uint8_t>(int bits);
template uint16_t BitArray::toNumber<uint16_t>(int bits);
template uint32_t BitArray::toNumber<uint32_t>(int bits);
template uint64_t BitArray::toNumber<uint64_t>(int bits);

template int8_t BitArray::toNumber<int8_t>(int bits);
template int16_t BitArray::toNumber<int16_t>(int bits);
template int32_t BitArray::toNumber<int32_t>(int bits);
template int64_t BitArray::toNumber<int64_t>(int bits);

} /* namespace barcodes */

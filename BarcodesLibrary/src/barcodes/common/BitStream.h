/*
 * BitStream.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include "BitArray.h"

namespace barcodes {
using namespace std;

class BitArray;

template<typename T>
class BitStream {
protected:
	BitArray &bitArray;
	size_t bitsToStream;
	size_t position;
	size_t _lastReadBits;
public:
	typedef T TYPE;

	BitStream(BitArray &bitArray, int bitsToStream = -1, size_t initPosition = 0)
		: bitArray(bitArray),
		  bitsToStream((bitsToStream == -1)? 8 * sizeof(T) : bitsToStream),
		  position((initPosition > bitArray.size())? 0 : initPosition),
		  _lastReadBits(0) {}
	virtual ~BitStream() {}


	BitStream& operator>> (T &codeword) {
		codeword = 0;
		size_t size = bitArray.size();

		size_t _position = position;
		for (uint32_t i = 0; (i < bitsToStream) && (position < size); i++, position++) {
			codeword += ((T)bitArray.getBit(position)) << i;
		}
		_lastReadBits = position - _position;

		return *this;
	}

	inline BitStream& operator() (int bitsToStream) {
		this->bitsToStream = bitsToStream;
		return *this;
	}

	inline void reset() {
		position = 0;
	}

	inline bool isEnd() {
		return position == bitArray.size();
	}

	inline size_t lastReadBits() {
		return _lastReadBits;
	}

	inline size_t getPosition() {
		return position;
	}

	inline void setPosition(size_t newPosition) {
		position = (newPosition > bitArray.size())? 0 : newPosition;
	}
};

} /* namespace barcodes */
#endif /* BITSTREAM_H_ */

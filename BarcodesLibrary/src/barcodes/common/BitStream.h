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
class BitStream_ {
protected:
	BitArray &bitArray;
	size_t bitsToStream;
	size_t position;
	size_t _lastReadBits;
public:
	typedef T TYPE;

	BitStream_(BitArray &bitArray, int bitsToStream = -1, size_t initPosition = 0)
		: bitArray(bitArray),
		  bitsToStream((bitsToStream == -1)? 8 * sizeof(T) : bitsToStream),
		  position((initPosition > bitArray.size())? 0 : initPosition),
		  _lastReadBits(0) {}
	virtual ~BitStream_() {}

	virtual BitStream_<T>& operator>> (T &codeword) = 0;

	inline virtual BitStream_<T>& operator() (int bitsToStream) {
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

template<typename T>
class BitStream:public BitStream_<T> {
protected:
	template <typename T2>
	inline BitStream_<T>& stream_in (T2 &codeword) {
		codeword = 0;
		size_t size = this->bitArray.size();

		size_t _position = this->position;
		for (uint32_t i = 0; (i < this->bitsToStream) && (this->position < size); i++, this->position++) {
			codeword += ((T2)this->bitArray.getBit(this->position)) << i;
		}
		this->_lastReadBits = this->position - _position;

		return *this;
	}
public:
	BitStream(BitArray &bitArray, int bitsToStream = -1, size_t initPosition = 0)
		: BitStream_<T>(bitArray, bitsToStream, initPosition)  {}

	inline BitStream<T>& operator() (int bitsToStream) {
		BitStream_<T>::operator ()(bitsToStream);
		return *this;
	}

	inline BitStream_<T>& operator>> (T &codeword) {
		return stream_in(codeword);
	}

	template <typename T2>
	inline BitStream<T>& operator>> (T2 &codeword) {
		return stream_in(codeword);
	}
};

template<typename T>
class BitStreamReverseCodeword:public BitStream_<T> {
protected:
	template <typename T2>
	inline BitStreamReverseCodeword<T>& stream_in (T2 &codeword) {
		codeword = 0;
		size_t size = this->bitArray.size();

		size_t _position = this->position;
		for (uint32_t i = 0; (i < this->bitsToStream) && (this->position < size); i++, this->position++) {
			if (i != 0)  codeword <<= 1;
			codeword += ((T2)this->bitArray.getBit(this->position)) & 0x01;
		}

		this->_lastReadBits = this->position - _position;

		return *this;
	}
public:
	BitStreamReverseCodeword(BitArray &bitArray, int bitsToStream = -1, size_t initPosition = 0)
		: BitStream_<T>(bitArray, bitsToStream, initPosition)  {}

	inline BitStreamReverseCodeword<T>& operator() (int bitsToStream) {
		BitStream_<T>::operator ()(bitsToStream);
		return *this;
	}

	inline BitStream_<T>& operator>> (T &codeword) {
		return stream_in(codeword);
	}

	template <typename T2>
	inline BitStreamReverseCodeword<T>& operator>> (T2 &codeword) {
		return stream_in(codeword);
	}
};

} /* namespace barcodes */
#endif /* BITSTREAM_H_ */

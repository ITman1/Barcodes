///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       BitStream.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines BitStream_ template and BitStream, BitStreamReverseCodeword
//             templates which derives from this BitStream_ template. Template
//             classes implements reading the bits from the bit array as a stream.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file BitStream.h
 *
 * @brief Defines BitStream_ template and BitStream, BitStreamReverseCodeword
 *        templates which derives from this BitStream_ template. Template
 *        classes implements reading the bits from the bit array as a stream.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include "BitArray.h"

namespace barcodes {
using namespace std;

/**
 * Class BitStream_ implements base behavior for stream which reads bits from
 * the bit array.
 */
template<typename T>
class BitStream_ {
protected:
	/**
	 * Reference to the bit array from which stream is reading.
	 */
	BitArray &bitArray;

	/**
	 * Number of bits to be streamed/read next time.
	 */
	size_t bitsToStream;

	/**
	 * Current stream position in the array.
	 */
	size_t position;

	/**
	 * Number bits which were streamed/read for the last time.
	 */
	size_t _lastReadBits;
public:
	typedef T TYPE; /**< Accessor for template type. */

	/**
	 * Constructs bit stream on the specified bit array and start position.
	 *
	 * @param bitArray Bit array to be used for streaming the bits.
	 * @param bitsToStream Number of bits to be streamed.
	 * @param initPosition Initial position in the bit array of the stream.
	 */
	BitStream_(BitArray &bitArray, int bitsToStream = -1, size_t initPosition = 0)
		: bitArray(bitArray),
		  bitsToStream((bitsToStream == -1)? 8 * sizeof(T) : bitsToStream),
		  position((initPosition > bitArray.size())? 0 : initPosition),
		  _lastReadBits(0) {}
	virtual ~BitStream_() {}

	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to this stream.
	 */
	virtual BitStream_<T>& operator>> (T &codeword) = 0;

	/**
	 * Sets the number of bits to be streamed the next time.
	 *
	 * @param bitsToStream Number of bits to be streamed the next time.
	 * @return Reference to this stream.
	 */
	inline virtual BitStream_<T>& operator() (int bitsToStream) {
		this->bitsToStream = bitsToStream;
		return *this;
	}

	/**
	 * Sets the position of stream to zero position in the bit array.
	 */
	inline void reset() {
		position = 0;
	}

	/**
	 * Tests whether the end of stream has been reached.
	 *
	 * @return True if it has been reached the end of stream.
	 */
	inline bool isEnd() {
		return position == bitArray.size();
	}

	/**
	 * Returns the number of bits which has been read for the last time.
	 *
	 * @return Number of bits which has been read for the last time.
	 */
	inline size_t lastReadBits() {
		return _lastReadBits;
	}

	/**
	 * Returns the position inside bit array from which is now stream reading.
	 *
	 * @return Position inside bit array from which is now stream reading.
	 */
	inline size_t getPosition() {
		return position;
	}

	/**
	 * Sets the position inside bit array from which is should the stream read the next time.
	 *
	 * @return Position inside bit array from which is should the stream read the next time.
	 */
	inline void setPosition(size_t newPosition) {
		position = (newPosition > bitArray.size())? 0 : newPosition;
	}
};

/**
 * Class implements the stream method which reads the first bits from the array as a low significant bits
 */
template<typename T>
class BitStream:public BitStream_<T> {
protected:
	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to this stream.
	 */
	template <typename T2>
	inline BitStream<T>& stream_in (T2 &codeword) {
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

	/**
	 * Sets the number of bits to be streamed the next time.
	 *
	 * @param bitsToStream Number of bits to be streamed the next time.
	 * @return Reference to this stream.
	 */
	inline BitStream<T>& operator() (int bitsToStream) {
		BitStream_<T>::operator ()(bitsToStream);
		return *this;
	}

	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to base stream.
	 */
	inline BitStream_<T>& operator>> (T &codeword) {
		return stream_in(codeword);
	}

	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to this stream.
	 */
	template <typename T2>
	inline BitStream<T>& operator>> (T2 &codeword) {
		return stream_in(codeword);
	}
};

/**
 * Class implements the stream method which reads the first bits from the array as a most significant bits
 */
template<typename T>
class BitStreamReverseCodeword:public BitStream_<T> {
protected:
	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to this stream.
	 */
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

	/**
	 * Sets the number of bits to be streamed the next time.
	 *
	 * @param bitsToStream Number of bits to be streamed the next time.
	 * @return Reference to this stream.
	 */
	inline BitStreamReverseCodeword<T>& operator() (int bitsToStream) {
		BitStream_<T>::operator ()(bitsToStream);
		return *this;
	}

	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to base stream.
	 */
	inline BitStream_<T>& operator>> (T &codeword) {
		return stream_in(codeword);
	}

	/**
	 * Streams/Reads bits from the bit array.
	 *
	 * @param codeword The result codeword constructed from the bits.
	 * @return Reference to this stream.
	 */
	template <typename T2>
	inline BitStreamReverseCodeword<T>& operator>> (T2 &codeword) {
		return stream_in(codeword);
	}
};

} /* namespace barcodes */
#endif /* BITSTREAM_H_ */

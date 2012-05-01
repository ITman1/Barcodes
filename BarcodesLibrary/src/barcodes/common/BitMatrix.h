///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       BitMatrix.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines BitArray class which holds bits inside OpenCV's matrix
//             and implements some other additional methods above it.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file BitMatrix.h
 *
 * @brief Defines BitArray class which holds bits inside OpenCV's matrix
 *        and implements some other additional methods above it.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef BITMATRIX_H_
#define BITMATRIX_H_

#include <opencv2/core/core.hpp>
#include "BitArray.h"

namespace barcodes {
using namespace cv;

/**
 * Class BitMatrix represents 2D matrix of the bits which are represented by
 * boolean type. Also implements sampling method by retrieving the bit
 * matrix from the image.
 */
class BitMatrix: public Mat_<bool> {
public:
	/**
	 * Short name for base class.
	 */
	typedef Mat_<bool> Base;

	BitMatrix() : Mat_<bool>() {}

	/**
	 * Constructs bit matrix and fills it by specified value.
	 *
	 * @param rows Rows of the matrix.
	 * @param cols Columns of the matrix.
	 * @param fill Value by which should be filled the matrix.
	 */
	BitMatrix(int rows, int cols, bool fill = false);

	/**
	 * Constructs bit matrix and fills it by specified value.
	 *
	 * @param size Size of the matrix.
	 * @param fill Value by which should be filled the matrix.
	 */
	BitMatrix(Size size, bool fill = false);

	/**
	 * Constructs bit matrix from another with the specified rectangle of interest.
	 *
	 * @param m Matrix from which should be constructed this matrix.
	 * @param roi Rectangle of interest which determines the range
	 *            from which should be constructed this matrix.
	 */
	BitMatrix(const BitMatrix& m, const Rect& roi);
	virtual ~BitMatrix() {}

	/**
	 * Operator () which acts same way as BitMatrix(const BitMatrix&, const Rect&) constructor.
	 *
	 * @param roi Rectangle of interest which determines the range
	 *            from which should be constructed this matrix.
	 * @return Constructed matrix.
	 */
	BitMatrix operator() (const Rect& roi) const;

	/**
	 * Returns bit on the specified row and column.
	 *
	 * @param row Row from which should be bit retrieved.
	 * @param col Column from which should be bit retrieved.
	 * @return Retrieved bit from the location.
	 */
	inline bool getBit(int row, int col) {
		return at(row, col);
	}

	/**
	 * Returns bit on the specified point.
	 *
	 * @param point Point from which should be bit retrieved.
	 * @return Retrieved bit from the location.
	 */
	inline bool getBit(Point point) {
		return at(point.y, point.x);
	}

	/**
	 * Sets bit on the specified location.
	 *
	 * @param row Row where should be bit set.
	 * @param col Column where should be bit set.
	 */
	inline void setBit(int row, int col, bool bit) {
		at(row, col) = bit;
	}

	/**
	 * Returns reference on the value at the specified location.
	 *
	 * @param row Row from which should be reference returned.
	 * @param col Column from which should be reference returned.
	 * @return Reference on some value of the matrix.
	 */
	inline bool &at(int row, int col) {
		return Mat::at<bool>(row, col);
	}

	/**
	 * Returns row as a bit array.
	 *
	 * @param row Position of the row which should be returned.
	 * @param rowArr Output bit array.
	 */
	void getRow(int row, BitArray &rowArr);

	/**
	 * Pushes row at the end of this matrix.
	 *
	 * @param rowArr Bit array which will be added at the end of the bit matrix.
	 */
	void pushRow(BitArray &rowArr);

	/**
	 * Removes column from the specified location.
	 *
	 * @param col Column which should be removed.
	 */
	void removeCol(int col);

	/**
	 * Clears bit matrix.
	 */
	void clear();

	/**
	 * Returns size of the bit matrix.
	 *
	 * @return Size of the bit matrix.
	 */
	Size size();

	/**
	 * Fills rectangle inside matrix by specified value.
	 *
	 * @param rects Vector of rectangles to be filled.
	 * @param fill Value by which should be rectangles filled.
	 */
	void fillRects(vector<Rect> &rects, bool fill = true);

	/**
	 * Masks whole array by mask bit matrix.
	 * Mask operation is the AND operator.
	 *
	 * @param mask Mask bit matrix by should be masked this bit matrix.
	 */
	void maskAND(BitMatrix &mask);

	/**
	 * Masks whole array by mask bit matrix.
	 * Mask operation is the XOR operator.
	 *
	 * @param mask Mask bit matrix by should be masked this bit matrix.
	 */
	void maskXOR(BitMatrix &mask);

	/**
	 * Constructs bit matrix from the image. Values of the bit matrix are sampled
	 * by sampling grid.
	 *
	 * @param img Image from which should be constructed the bit matrix.
	 * @param sampleSize Size of the output bit matrix.
	 * @param outMatrix Output bit matrix.
	 * @param roi Rectangle of interest from which should be sampled.
	 * @param paddingRatio Padding of each sampled cell. | (paddingRatio/2) ((1-paddingRatio) == sampleRatio) (paddingRatio/2) |
	 */
	static void fromImage(Mat img, Size sampleSize, BitMatrix &outMatrix, Rect roi = Rect(-1, -1, -1, -1), double paddingRatio = 0.4);
};

} /* namespace barcodes */
#endif /* BITMATRIX_H_ */

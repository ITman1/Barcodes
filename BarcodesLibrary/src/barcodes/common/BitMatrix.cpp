///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       BitMatrix.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of BitArray class which holds bits inside OpenCV's
//             matrix and implements some other additional methods above it.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file BitMatrix.cpp
 *
 * @brief Defines members of BitArray class which holds bits inside OpenCV's
 *        matrix and implements some other additional methods above it.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <opencv2/imgproc/imgproc.hpp>

#include "BitMatrix.h"

namespace barcodes {

/**
 * Constructs bit matrix and fills it by specified value.
 *
 * @param rows Rows of the matrix.
 * @param cols Columns of the matrix.
 * @param fill Value by which should be filled the matrix.
 */
BitMatrix::BitMatrix(int rows, int cols, bool fill) : Mat_<bool>(rows, cols, fill) {}

/**
 * Constructs bit matrix and fills it by specified value.
 *
 * @param size Size of the matrix.
 * @param fill Value by which should be filled the matrix.
 */
BitMatrix::BitMatrix(Size size, bool fill) : Mat_<bool>(size, fill) { }

/**
 * Constructs bit matrix from another with the specified rectangle of interest.
 *
 * @param m Matrix from which should be constructed this matrix.
 * @param roi Rectangle of interest which determines the range
 *            from which should be constructed this matrix.
 */
BitMatrix::BitMatrix(const BitMatrix& m, const Rect& roi) : Mat_<bool>(m, roi) { }

/**
 * Operator () which acts same way as BitMatrix(const BitMatrix&, const Rect&) constructor.
 *
 * @param roi Rectangle of interest which determines the range
 *            from which should be constructed this matrix.
 * @return Constructed matrix.
 */
BitMatrix BitMatrix::operator() (const Rect& roi) const {
	return BitMatrix(*this, roi);
}

/**
 * Returns row as a bit array.
 *
 * @param row Position of the row which should be returned.
 * @param rowArr Output bit array.
 */
void BitMatrix::getRow(int row, BitArray &rowArr) {
	Mat rowMat = this->row(row);
	MatIterator_<bool> rowIter;

	rowArr.clear();
	for (rowIter = rowMat.begin<bool>(); rowIter != rowMat.end<bool>(); rowIter++) {
		rowArr.push_back(*rowIter);
	}
}

/**
 * Pushes row at the end of this matrix.
 *
 * @param rowArr Bit array which will be added at the end of the bit matrix.
 */
void BitMatrix::pushRow(BitArray &rowArr) {
	if ((int)rowArr.size() != cols) return;

	resize(rows + 1);

	for (int i = 0; i < cols;i++) {
		at(rows - 1, i) = rowArr.at(i);
	}
}

/**
 * Clears bit matrix.
 */
void BitMatrix::clear() {
	release();
}

/**
 * Returns size of the bit matrix.
 *
 * @return Size of the bit matrix.
 */
Size BitMatrix::size() {
	return Size(cols, rows);
}

/**
 * Fills rectangle inside matrix by specified value.
 *
 * @param rects Vector of rectangles to be filled.
 * @param fill Value by which should be rectangles filled.
 */
void BitMatrix::fillRects(vector<Rect> &rects, bool fill) {
	vector<Rect>::iterator iter;

	for (iter = rects.begin(); iter != rects.end(); iter++) {
		rectangle(*this, *iter, Scalar(fill), CV_FILLED);
	}
}

/**
 * Masks whole array by mask bit matrix.
 * Mask operation is the AND operator.
 *
 * @param mask Mask bit matrix by should be masked this bit matrix.
 */
void BitMatrix::maskAND(BitMatrix &mask) {
	if (mask.size() != this->size()) return;

	iterator iter = begin();
	iterator iter_end = end();
	iterator iter2 = mask.begin();

	for(; iter != iter_end; iter++, iter2++ ) {
		*iter &= *iter2;
	}
}

/**
 * Masks whole array by mask bit matrix.
 * Mask operation is the XOR operator.
 *
 * @param mask Mask bit matrix by should be masked this bit matrix.
 */
void BitMatrix::maskXOR(BitMatrix &mask) {
	if (mask.size() != this->size()) return;

	iterator iter = begin();
	iterator iter_end = end();
	iterator iter2 = mask.begin();

	for(; iter != iter_end; iter++, iter2++ ) {
		*iter ^= *iter2;
	}
}

/**
 * Removes column from the specified location.
 *
 * @param col Column which should be removed.
 */
void BitMatrix::removeCol(int col) {
	Base _cloned = this->clone();
	if (cols > 1) {
		for (int row = 0; row < rows; row++) {
			bool *remElem = &_cloned[row][col];
			bool *currElem = &_cloned[row][cols - 1];

			bool swap;
			bool prevElem = *currElem;
			while (remElem != currElem) {
				--currElem;
				swap = prevElem;
				prevElem = *currElem;
				*currElem = swap;
			}
		}
	}

	Base _stripped = _cloned(Rect(0, 0, cols - 1, rows));

	create(_stripped.rows, _stripped.cols);

	iterator iter = begin();
	iterator iter_end = end();
	iterator iter2 = _stripped.begin();

	for(; iter != iter_end; iter++, iter2++ ) {
		*iter = *iter2;
	}
}

/**
 * Constructs bit matrix from the image. Values of the bit matrix are sampled
 * by sampling grid.
 *
 * @param img Image from which should be constructed the bit matrix.
 * @param sampleSize Size of the output bit matrix.
 * @param outMatrix Output bit matrix.
 * @param roi Rectangle of interest from which should be sampled.
 * @param marginRatio Margin of each sampled cell. | (marginRatio/2) ((1-marginRatio) == sampleRatio) (marginRatio/2) |
 */
void BitMatrix::fromImage(Mat img, Size sampleGridSize, BitMatrix &outMatrix, Rect roi, double marginRatio) {
	Mat _img;
	if (roi.height != -1) {
		_img = img(roi);
	} else {
		_img = img;
	}

	int _rows = sampleGridSize.height;
	int _cols = sampleGridSize.width;
	double _rowSize = _img.rows / (double)_rows;
	double _colSize = _img.cols / (double)_cols;
	double _marginColSize = ceil(_colSize * (1 - marginRatio));
	double _marginRowSize = ceil(_rowSize * (1 - marginRatio));
	double _marginColOffset = _colSize * (marginRatio / 2.0);
	double _marginRowOffset = _rowSize * (marginRatio / 2.0);
	int _pixelsPerModule = (int)_marginColSize * (int)_marginRowSize;

	if (_rows >= 1 && _cols >= 1) {
		outMatrix = BitMatrix(0, _cols);

		for (int i = 0; i < _rows; i++) {
			BitArray _rowArr;
			_rowArr.clear();

			for (int j = 0; j < _cols; j++) {
				int _blackPixels = cv::countNonZero(_img(Rect(_marginColOffset + j * _colSize, _marginRowOffset + i * _rowSize, _marginColSize, _marginRowSize)));
				bool _notBit = round(_blackPixels / (double)_pixelsPerModule);
				_rowArr.pushBit( ! _notBit );
			}

			outMatrix.pushRow(_rowArr);
		}

	} else {
		outMatrix.clear();
	}
}

} /* namespace barcodes */

/*
 * GridSampler.cpp
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#include "BitMatrix.h"

namespace barcodes {

BitMatrix::BitMatrix(bool fill) : Mat_<bool>(0, 0, fill) {}

BitMatrix::BitMatrix(int rows, int cols, bool fill) : Mat_<bool>(rows, cols, fill) {}

BitMatrix::BitMatrix(Size size, bool fill) : Mat_<bool>(size, fill) { }

BitMatrix::BitMatrix(const BitMatrix& m, const Rect& roi) : Mat_<bool>(m, roi) {
	int i = 0;
}

BitMatrix BitMatrix::operator() (const Rect& roi) const {
	return BitMatrix(*this, roi);
}

void BitMatrix::getRow(int row, BitArray &rowArr) {
	Mat rowMat = this->row(row);
	MatIterator_<bool> rowIter;

	rowArr.clear();
	for (rowIter = rowMat.begin<bool>(); rowIter != rowMat.end<bool>(); rowIter++) {
		rowArr.push_back(*rowIter);
	}
}

void BitMatrix::pushRow(BitArray &rowArr) {
	if (rowArr.size() != cols) return;

	resize(rows + 1);

	for (int i = 0; i < cols;i++) {
		at(rows - 1, i) = rowArr.at(i);
	}
}

void BitMatrix::clear() {
	release();
}

Size BitMatrix::size() {
	return Size(cols, rows);
}

void BitMatrix::fillRects(vector<Rect> &rects, bool fill) {
	vector<Rect>::iterator iter;

	for (iter = rects.begin(); iter != rects.end(); iter++) {
		rectangle(*this, *iter, Scalar(fill), CV_FILLED);
	}
}

void BitMatrix::maskAND(BitMatrix &mask) {
	if (mask.size() != this->size()) return;

	iterator iter;
	iterator iter_end = end();
	iterator iter2;

	for(; iter != iter_end; iter++, iter2++ ) {
		*iter &= *iter2;
	}
}

void BitMatrix::maskXOR(BitMatrix &mask) {
	if (mask.size() != this->size()) return;

	iterator iter;
	iterator iter_end = end();
	iterator iter2;

	for(; iter != iter_end; iter++, iter2++ ) {
		*iter ^= *iter2;
	}
}

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

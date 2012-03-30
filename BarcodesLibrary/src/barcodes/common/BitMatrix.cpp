/*
 * GridSampler.cpp
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#include "BitMatrix.h"

namespace barcodes {

BitMatrix::BitMatrix() : Mat(0, 0, CV_8UC1, Scalar(false)) {}

BitMatrix::BitMatrix(int rows, int cols) : Mat(rows, cols, CV_8UC1, Scalar(false)) {}

BitMatrix::BitMatrix(Size size) : Mat(size, CV_8UC1, Scalar(false)) { }

void BitMatrix::getRow(int row, BitArray &rowArr) {
	Mat rowMat = this->row(row);
	MatIterator_<uchar> rowIter;

	rowArr.clear();
	for (rowIter = rowMat.begin<uchar>(); rowIter != rowMat.end<uchar>(); rowIter++) {
		rowArr.push_back(*rowIter);
	}
}

void BitMatrix::pushRow(BitArray &rowArr) {
	if (rowArr.size() != cols) return;

	resize(rows + 1);

	for (int i = 0; i < cols;i++) {
		at<uchar>(rows - 1, i) = rowArr.at(i);
	}
}

void BitMatrix::clear() {
	release();
}

void BitMatrix::fillRects(vector<Rect> &rects) {
	vector<Rect>::iterator iter;

	for (iter = rects.begin(); iter != rects.end(); iter++) {
		rectangle(*this, *iter, Scalar(true), CV_FILLED);
	}
}

void BitMatrix::fromImage(Mat img, Size sampleGridSize, BitMatrix &outMatrix, Rect roi) {
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
	int _pixelsPerModule = (int)_rowSize * (int)_colSize;

	if (_rows >= 1 && _cols >= 1) {
		outMatrix = BitMatrix(0, _cols);

		for (int i = 0; i < _rows; i++) {
			BitArray _rowArr;
			_rowArr.clear();

			for (int j = 0; j < _cols; j++) {
				int _blackPixels = cv::countNonZero(_img(Rect(j * _colSize, i * _rowSize, _colSize, _rowSize)));
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

/*
 * BitMatrix.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#ifndef BITMATRIX_H_
#define BITMATRIX_H_

#include <opencv2/core/core.hpp>
#include "BitArray.h"

namespace barcodes {
using namespace cv;
class BitMatrix: public Mat {
public:
	BitMatrix();
	BitMatrix(int rows, int cols);
	BitMatrix(Size size);
	virtual ~BitMatrix() {}

	inline bool getBit(int row, int col) {
		return at<uchar>(row, col);
	}

	inline bool getBit(Point point) {
		return at<uchar>(point.y, point.x);
	}

	inline void setBit(int row, int col, bool bit) {
		at<uchar>(row, col) = bit;
	}

	void getRow(int row, BitArray &rowArr);
	void pushRow(BitArray &rowArr);
	void clear();
	void fillRects(vector<Rect> &rects);

	static void fromImage(Mat img, Size sampleSize, BitMatrix &outMatrix, Rect roi = Rect(-1, -1, -1, -1));
};

} /* namespace barcodes */
#endif /* BITMATRIX_H_ */

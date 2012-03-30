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
	BitMatrix(bool fill = false);
	BitMatrix(int rows, int cols, bool fill = false);
	BitMatrix(Size size, bool fill = false);
	BitMatrix(const BitMatrix& m, const Rect& roi);
	virtual ~BitMatrix() {}

	BitMatrix operator() (const Rect& roi) const;

	inline bool getBit(int row, int col) {
		return at<uchar>(row, col);
	}

	inline bool getBit(Point point) {
		return at<uchar>(point.y, point.x);
	}

	inline void setBit(int row, int col, bool bit) {
		at<uchar>(row, col) = bit;
	}

	/*inline bool &at(int row, int col) {
		return at<uchar>(row, col);
	}*/

	void getRow(int row, BitArray &rowArr);
	void pushRow(BitArray &rowArr);
	void clear();
	Size size();
	void fillRects(vector<Rect> &rects, bool fill = true);
	void maskAND(BitMatrix &mask);
	void maskXOR(BitMatrix &mask);

	static void fromImage(Mat img, Size sampleSize, BitMatrix &outMatrix, Rect roi = Rect(-1, -1, -1, -1));
};

} /* namespace barcodes */
#endif /* BITMATRIX_H_ */

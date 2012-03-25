/*
 * BuildHelper.cpp
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "QrBuildHelper.h"

namespace barcodes {

Mat QrBuildHelper::buildQrMark(int size) {
	int dotSize = size / 7;
	Mat mark = Mat::zeros(Size(size, size), CV_8UC1);

	rectangle(mark, Rect(dotSize, dotSize, 5 * dotSize, 5 * dotSize), Scalar(255), 1);
	rectangle(mark, Rect(2 * dotSize - 1, 2 * dotSize - 1, 3 * dotSize + 2, 3 * dotSize + 2), Scalar(255), 1);
	floodFill(mark, Point(dotSize + 1, dotSize + 1), Scalar(255));

	return mark;
}

} /* namespace barcodes */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrBuildHelper.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrBuildHelper class which groups helper
//             static methods for building the parts of the QR code.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBuildHelper.cpp
 *
 * @brief Defines members of QrBuildHelper class which groups helper
 *        static methods for building the parts of the QR code.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "QrBuildHelper.h"

namespace barcodes {

/**
 * Builds the finder pattern with the specified size.
 *
 * @param size Size of the finder mark.
 * @return Image with the build finder mark.
 */
Mat QrBuildHelper::buildQrMark(int size) {
	double dotSize = size / 7.0;
	Mat mark = Mat::zeros(Size(size, size), CV_8UC1);

	rectangle(mark, Rect(dotSize, dotSize, 5 * dotSize, 5 * dotSize), Scalar(255), 1);
	rectangle(mark, Rect(2 * dotSize - 1, 2 * dotSize - 1, 3 * dotSize + 2, 3 * dotSize + 2), Scalar(255), 1);
	floodFill(mark, Point(dotSize + 1, dotSize + 1), Scalar(255));

	return mark;
}

/**
 * Builds the alignment pattern with the specified size.
 *
 * @param size Size of the alignment mark.
 * @return Image with the built alignment mark.
 */
Mat QrBuildHelper::buildAlignementMark(int size) {
	double dotSize = size / 5.0;
	Mat mark = Mat::zeros(Size(size, size), CV_8UC1);

	rectangle(mark, Rect(dotSize, dotSize, 3 * dotSize, 3 * dotSize), Scalar(255), 1);
	rectangle(mark, Rect(2 * dotSize - 1, 2 * dotSize - 1, dotSize + 2, dotSize + 2), Scalar(255), 1);
	floodFill(mark, Point(dotSize + 1, dotSize + 1), Scalar(255));

	return mark;
}

} /* namespace barcodes */

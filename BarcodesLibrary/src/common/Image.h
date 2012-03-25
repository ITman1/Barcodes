/*
 * Image.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_IMAGE_H_
#define BARCODES_IMAGE_H_

#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace std;
using namespace cv;

#define FORMAT2TYPE(colorFormat) (\
	(colorFormat == IMAGE_COLOR_RGB)?         CV_8UC3 :\
	(colorFormat == IMAGE_COLOR_GRAYSCALE)?   CV_8UC1 :\
	(colorFormat == IMAGE_COLOR_YUV420sp)?    CV_8UC1 :\
CV_8UC1)

enum ImageColors {
	IMAGE_COLOR_RGB				= 0x01,
	IMAGE_COLOR_GRAYSCALE		= 0x02,
	IMAGE_COLOR_YUV420sp		= 0x03
};

class Image: public Mat {
private:
	int colorFormat;
public:
	Image(Size size, int colorFormat = IMAGE_COLOR_RGB)
		: Mat(size, FORMAT2TYPE(colorFormat)), colorFormat(colorFormat) {}
	Image(const Mat &img, int colorFormat)
		: Mat(img), colorFormat(colorFormat) {}
	Image(Size size, void* data, int colorFormat, size_t _step = AUTO_STEP)
		: Mat(size, FORMAT2TYPE(colorFormat), data, _step), colorFormat(colorFormat) {}

	Image(int _rows, int _cols, void* data, int colorFormat, size_t _step = AUTO_STEP)
		: Mat(_rows, _cols, FORMAT2TYPE(colorFormat), data, _step), colorFormat(colorFormat) {}

	static Image fromFileRGB(string filename);
	static Image fromFileGrayscale(string filename);

	int getColorFormat();
	bool convertColorFormat(int newColorFormat);
};

template <typename PixelDataType>
class Image_: public Image {
public:
	typedef Vec<uchar, 3> RGB;
	typedef Vec<uchar, 1> Grayscale;

	Image_(int rows, int cols)
		: Mat(rows, cols, DataType<PixelDataType>::type) {}

	Image_(Size size)
		: Mat(size, DataType<PixelDataType>::type) {}
};

} /* namespace barcodes */
#endif /* IMAGE_H_ */

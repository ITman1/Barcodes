/*
 * Image.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_IMAGE_H_
#define BARCODES_IMAGE_H_

#include <string>
#include <opencv2/core/core.hpp>

#include "types.h"

using namespace std;
using namespace cv;

namespace barcodes {

	enum ImageColors {
		IMAGE_COLOR_RGB				= 0x01,
		IMAGE_COLOR_GRAYSCALE		= 0x02,
		IMAGE_COLOR_YCBCR			= 0x03
	};

	class Image: public Mat {
	private:
		int colorFormat;
	public:
		Image(Size size, int type, int colorFormat = IMAGE_COLOR_RGB)
			: Mat(size, type), colorFormat(colorFormat) {}
		Image(Mat &mat, int colorFormat = IMAGE_COLOR_RGB)
			: Mat(mat), colorFormat(colorFormat) {}
		Image(Size size, int type, void* data, size_t step = AUTO_STEP, int colorFormat = IMAGE_COLOR_RGB)
			: Mat(size, type, data, step), colorFormat(colorFormat) {}

		Image(ByteArray &image, bool copyImage = false, int colorFormat = IMAGE_COLOR_RGB)
			: Mat(image, copyImage), colorFormat(colorFormat) {}

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

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Image.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Image class which extends OpenCV's Mat class about
//             some color conversion/load/save methods.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Image.h
 *
 * @brief Defines the Image class which extends OpenCV's Mat class about
 *        some color conversion/load/save methods.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
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

// Converts color format type to corresponding data type
#define FORMAT2TYPE(colorFormat) (\
	(colorFormat == IMAGE_COLOR_RGB)?         CV_8UC3 :\
	(colorFormat == IMAGE_COLOR_GRAYSCALE)?   CV_8UC1 :\
	(colorFormat == IMAGE_COLOR_YUV420sp)?    CV_8UC1 :\
CV_8UC1)

/**
 * Enumeration of supported color formats.
 */
enum ImageColors {
	IMAGE_COLOR_RGB				= 0x01,    //!< IMAGE_COLOR_RGB
	IMAGE_COLOR_GRAYSCALE		= 0x02,//!< IMAGE_COLOR_GRAYSCALE
	IMAGE_COLOR_YUV420sp		= 0x03  //!< IMAGE_COLOR_YUV420sp
};

/**
 * Definition of the Image class which wraps OpenCV image readers/decoders
 * and makes easier conversions from one color format to another one.
 *
 * @todo Add unsupported format and bad allocated image exceptions.
 */
class Image: public Mat {
private:
	/** Current color format of this image */
	int colorFormat;
public:
	/**
	 * Creates an image of specified size and color format.
	 *
	 * @param size Size of new image.
	 * @param colorFormat Color format of new image.
	 */
	Image(Size size = Size(), int colorFormat = IMAGE_COLOR_RGB)
		: Mat(size, FORMAT2TYPE(colorFormat)), colorFormat(colorFormat) {}

	/**
	 * Creates an image from already existing image inside Mat structure.
	 *
	 * @param img Existing image.
	 * @param colorFormat Color format of the existing image.
	 */
	Image(const Mat &img, int colorFormat)
		: Mat(img), colorFormat(colorFormat) {}

	/**
	 * Creates an image from user allocated image.
	 *
	 * @param size Size of image.
	 * @param colorFormat Color format of image.
	 * @param step Parameter that specifies the number of bytes which image row occupies.
	 */
	Image(Size size, void* data, int colorFormat, size_t _step = AUTO_STEP)
		: Mat(size, FORMAT2TYPE(colorFormat), data, _step), colorFormat(colorFormat) {}

	/**
	 * Creates an image from user allocated image.
	 *
	 * @param _rows Number of rows of the image.
	 * @param _cols Number of columns of the image.
	 * @param colorFormat Color format of image.
	 * @param step Parameter that specifies the number of bytes which image row occupies.
	 */
	Image(int _rows, int _cols, void* data, int colorFormat, size_t _step = AUTO_STEP)
		: Mat(_rows, _cols, FORMAT2TYPE(colorFormat), data, _step), colorFormat(colorFormat) {}

	/**
	 * Loads the image from the file in RGB color mode.
	 *
	 * @param filename Filename of the file to be loaded.
	 * @return Loaded image.
	 */
	static Image fromFileRGB(string filename);

	/**
	 * Loads the image from the file in grayscale color mode.
	 *
	 * @param filename Filename of the file to be loaded.
	 * @return Loaded image.
	 */
	static Image fromFileGrayscale(string filename);

	/**
	 * Decodes and loads the image from the byte array in RGB color mode.
	 *
	 * @param arr Byte array from which is loaded the image.
	 * @return Loaded image.
	 */
	static Image fromByteArrayRGB(vector<uint8_t> &arr);

	/**
	 * Decodes and loads the image from the byte array in grayscale color mode.
	 *
	 * @param arr Byte array from which is loaded the image.
	 * @return Loaded image.
	 */
	static Image fromByteArrayGrayscale(vector<uint8_t> &arr);

	/**
	 * Returns color format of this image.
	 *
	 * @return Color format of this image.
	 */
	int getColorFormat();

	/**
	 * Converts image from one color format to another one, if supported.
	 *
	 * @param newColorFormat Format to which should be an image converted.
	 * @return True if this conversion method exists and ends with success, otherwise false.
	 */
	bool convertColorFormat(int newColorFormat);
};

} /* namespace barcodes */
#endif /* IMAGE_H_ */

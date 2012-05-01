///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Image.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Image class members. Image class extends OpenCV's
//             Mat class about some color conversion/load/save methods.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Image.cpp
 *
 * @brief Defines the Image class which extends OpenCV's Mat class about
 *        some color conversion/load/save methods.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "Image.h"
#include "../debug.h"

using namespace barcodes;

/**
 * Loads the image from the file in RGB color mode.
 *
 * @param filename Filename of the file to be loaded.
 * @param img Optional parameter where to store the image.
 * @return Loaded image.
 */
Image Image::fromFileRGB(string filename, Mat *img) {
	if (img == 0) {
		Mat mat = imread(filename, CV_LOAD_IMAGE_COLOR);
		return Image(mat, IMAGE_COLOR_RGB);
	} else {
		*img = imread(filename, CV_LOAD_IMAGE_COLOR);
		return Image(*img, IMAGE_COLOR_RGB);
	}
}

/**
 * Loads the image from the file in grayscale color mode.
 *
 * @param filename Filename of the file to be loaded.
 * @param img Optional parameter where to store the image.
 * @return Loaded image.
 */
Image Image::fromFileGrayscale(string filename, Mat *img) {
	if (img == 0) {
		Mat mat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		return Image(mat, IMAGE_COLOR_GRAYSCALE);
	} else {
		*img = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		return Image(*img, IMAGE_COLOR_GRAYSCALE);
	}
}

/**
 * Decodes and loads the image from the byte array in RGB color mode.
 *
 * @param arr Byte array from which is loaded the image.
 * @return Loaded image.
 */
Image Image::fromByteArrayRGB(vector<uint8_t> &arr) {
	Mat mat = imdecode(Mat(arr), CV_LOAD_IMAGE_COLOR);
	return Image(mat, IMAGE_COLOR_RGB);
}

Image Image::fromByteArrayRGB(void *buffer, int length) {
	Mat mat = imdecode(Mat(Size(length, 1), CV_8UC1, buffer), CV_LOAD_IMAGE_COLOR);
	return Image(mat, IMAGE_COLOR_RGB);
}

/**
 * Decodes and loads the image from the byte array in grayscale color mode.
 *
 * @param arr Byte array from which is loaded the image.
 * @return Loaded image.
 */
Image Image::fromByteArrayGrayscale(vector<uint8_t> &arr) {
	Mat mat = imdecode(Mat(arr), CV_LOAD_IMAGE_GRAYSCALE);
	return Image(mat, IMAGE_COLOR_GRAYSCALE);
}
Image Image::fromByteArrayGrayscale(void *buffer, int length) {
	Mat mat = imdecode(Mat(Size(length, 1), CV_8UC1, buffer), IMAGE_COLOR_GRAYSCALE);
	return Image(mat, IMAGE_COLOR_GRAYSCALE);
}

/**
 * Converts image from one color format to another one, if supported.
 *
 * @param newColorFormat Format to which should be an image converted.
 * @return True if this conversion method exists and ends with success, otherwise false.
 */
bool Image::convertColorFormat(int newColorFormat) {
	return convertColorFormat(*this, *this, newColorFormat);
}

/**
 * Converts image from one color format to another one, if supported.
 *
 * @param srcImage Source image.
 * @param dstImage Destination image.
 * @param newColorFormat Format to which should be an image converted.
 * @return True if this conversion method exists and ends with success, otherwise false.
 */
bool Image::convertColorFormat(Image &srcImage, Image &dstImage, int newColorFormat) {
	Mat destImage;

	if (srcImage.getColorFormat() == newColorFormat) return true;

	switch (srcImage.getColorFormat()) {
	case IMAGE_COLOR_RGB:
		switch (newColorFormat) {
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(srcImage, destImage, CV_RGB2GRAY);
			break;
		case IMAGE_COLOR_YUV420sp:
			//cvtColor(srcImage, destImage, );
			break;
		}
		break;
	case IMAGE_COLOR_GRAYSCALE:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(srcImage, destImage, CV_GRAY2RGB);
			break;
		case IMAGE_COLOR_YUV420sp:
			//cvtColor(srcImage, destImage, CV_GRAY2RGB);
			break;
		}
		break;
	case IMAGE_COLOR_YUV420sp:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(srcImage, destImage, CV_YUV420sp2RGB, 3);
			break;
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(srcImage, destImage, CV_YUV420sp2RGB, 3);
			cvtColor(srcImage, destImage, CV_YUV420sp2RGB);
			cvtColor(destImage, destImage, CV_RGB2GRAY);
			break;
		}
		break;
	}

	if (destImage.data != NULL) {
		dstImage = Image(destImage, newColorFormat);
		return true;
	} else {
		return false;
	}
}

/**
 * Returns color format of this image.
 *
 * @return Color format of this image.
 */
int Image::getColorFormat() {
	return colorFormat;
}

Image& Image::operator() (void *data, Size size, int colorFormat) {
	switch (colorFormat) {
	case IMAGE_COLOR_YUV420sp:
		if ((this->rows != size.height * 3/2) || (this->type() != FORMAT2TYPE(IMAGE_COLOR_YUV420sp))
				|| (this->cols != size.width) || this->colorFormat != colorFormat) {
			DEBUG_PRINT("Image.cpp", "operator() CREATE IMAGE");
			*this = Image(size.height * 3/2, size.width, (void *)data, colorFormat);
		} else {
			DEBUG_PRINT("Image.cpp", "operator() USING ALLOCATED IMAGE");
			this->data = (uchar *)data;
		}
		break;
	}

	return *this;
}



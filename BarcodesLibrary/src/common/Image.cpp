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

using namespace barcodes;

/**
 * Loads the image from the file in RGB color mode.
 *
 * @param filename Filename of the file to be loaded.
 * @return Loaded image.
 */
Image Image::fromFileRGB(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_COLOR);
	return Image(mat, IMAGE_COLOR_RGB);
}

/**
 * Loads the image from the file in grayscale color mode.
 *
 * @param filename Filename of the file to be loaded.
 * @return Loaded image.
 */
Image Image::fromFileGrayscale(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	return Image(mat, IMAGE_COLOR_GRAYSCALE);
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

/**
 * Converts image from one color format to another one, if supported.
 *
 * @param newColorFormat Format to which should be an image converted.
 * @return True if this conversion method exists and ends with success, otherwise false.
 */
bool Image::convertColorFormat(int newColorFormat) {
	Mat destImage;

	if (getColorFormat() == newColorFormat) return true;

	switch (colorFormat) {
	case IMAGE_COLOR_RGB:
		switch (newColorFormat) {
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(*this, destImage, CV_RGB2GRAY);
			break;
		case IMAGE_COLOR_YUV420sp:
			//cvtColor(*this, destImage, );
			break;
		}
		break;
	case IMAGE_COLOR_GRAYSCALE:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(*this, destImage, CV_GRAY2RGB);
			break;
		case IMAGE_COLOR_YUV420sp:
			//cvtColor(*this, destImage, CV_GRAY2RGB);
			break;
		}
		break;
	case IMAGE_COLOR_YUV420sp:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(*this, destImage, CV_YUV420sp2RGB, 3);
			break;
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(*this, destImage, CV_YUV420sp2RGB, 3);
			cvtColor(*this, destImage, CV_YUV420sp2RGB);
			cvtColor(destImage, destImage, CV_RGB2GRAY);
			break;
		}
		break;
	}

	if (destImage.data != NULL) {
		*this = Image(destImage, newColorFormat);
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

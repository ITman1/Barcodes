/*
 * Image.cpp
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#include <opencv2/highgui/highgui.hpp>

#include "Image.h"

using namespace cv;
using namespace barcodes;

Image Image::fromFileRGB(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_COLOR);
	return Image(mat);
}

Image Image::fromFileGrayscale(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	return Image(mat);
}

bool Image::convertColorFormat(int newColorFormat) {
	Mat destImage;

	switch (colorFormat) {
	case IMAGE_COLOR_RGB:
		switch (newColorFormat) {
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(*this, destImage, CV_RGB2GRAY);
			break;
		case IMAGE_COLOR_YCBCR:
			cvtColor(*this, destImage, CV_RGB2YCrCb);
			break;
		}
		break;
	case IMAGE_COLOR_GRAYSCALE:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(*this, destImage, CV_GRAY2RGB);
			break;
		case IMAGE_COLOR_YCBCR:
			cvtColor(*this, destImage, CV_GRAY2RGB);
			cvtColor(destImage, destImage, CV_RGB2YCrCb);
			break;
		}
		break;
	case IMAGE_COLOR_YCBCR:
		switch (newColorFormat) {
		case IMAGE_COLOR_RGB:
			cvtColor(*this, destImage, CV_YCrCb2RGB);
			break;
		case IMAGE_COLOR_GRAYSCALE:
			cvtColor(*this, destImage, CV_YCrCb2RGB);
			cvtColor(destImage, destImage, CV_RGB2GRAY);
			break;
		}
		break;
	}

	if (destImage.data != NULL) {
		*this = destImage;
		return true;
	} else {
		return false;
	}
}

int Image::getColorFormat() {
	return colorFormat;
}

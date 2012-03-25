/*
 * Image.cpp
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#include "Image.h"

using namespace barcodes;

Image Image::fromFileRGB(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_COLOR);
	return Image(mat, IMAGE_COLOR_RGB);
}

Image Image::fromFileGrayscale(string filename) {
	Mat mat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	return Image(mat, IMAGE_COLOR_GRAYSCALE);
}

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

int Image::getColorFormat() {
	return colorFormat;
}

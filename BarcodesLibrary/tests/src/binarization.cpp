// Encoding: UTF-8

/**
 * File:      binarization.cpp
 * Date:      March 2012
 * Project:   Barcodes Library
 * Author:    Radim Loskot
 * Brief:     XXXXXXXXXXXXXXXXXXXXXXXX @todo
 *            XXXXXXXXXXXXXXXXXXXXXXXX @todo
 */

/**
 * @file binarization.cpp
 *
 * @brief XXXXXXXXXXXXXXXXXXXXXXXXXXXX @todo
 * @brief XXXXXXXXXXXXXXXXXXXXXXXXXXXX @todo
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>

#include <BarcodesLibrary.h>

using namespace barcodes;

int main() {
	QrBarcode barcode;
	Image image = Image::fromFileGrayscale("../img/2012-03-08 14.34.03.jpg");
	barcode.detect(image);
	//image.data;
	std::cout << endl << "END";
    return 0;
}

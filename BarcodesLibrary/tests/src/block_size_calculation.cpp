///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       block_size_calculation.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Determines the proper values of the curve for generating the size
//             of the block for adaptive threshold.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file block_size_calculation.cpp
 *
 * @brief Determines the proper values of the curve for generating the size
 *        of the block for adaptive threshold.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <dirent.h>

#include <barlib/barcodes/qr/QrBarcode.h>

using namespace barcodes;

string IntToStr(int number) {
   stringstream ss;
   ss << number;
   return ss.str();
}

string DoubleToStr(double number) {
   stringstream ss;
   ss << number;
   return ss.str();
}

int main() {
    mkdir("out");
    mkdir("out/block_size_calculation");
    mkdir("out/block_size_calculation/gens");
    mkdir("out/block_size_calculation/refs");
    mkdir("out/block_size_calculation/srcs");

	// JUST GENERATING THE REFERENCE AND SOURCE FILES
	Image image_src = Image::fromFileGrayscale("img/block_size_calculation/src_mean_block_size.jpg");
	Image image_ref = Image::fromFileGrayscale("img/block_size_calculation/ref_mean_block_size.jpg");
	 for (int i = 5000; i > 0; i -= 100) {
		resize(image_src, image_src, Size(i, (4135 / (double)5000) * i));
		imwrite("out/block_size_calculation/srcs/src_size_" +  IntToStr(i) + ".jpg", image_src);
		resize(image_ref, image_ref, Size(i, (4135 / (double)5000) * i));
		imwrite("out/block_size_calculation/refs/ref_size_" +  IntToStr(i) + ".jpg", image_ref);
	}

	// GENERATING THE OUTPUTS
	for (int i = 5000; i > 0; i -= 100) {
		Image image = Image::fromFileGrayscale("out/block_size_calculation/srcs/src_size_" +  IntToStr(i) + ".jpg");
		for (int j = 3; j < 800; j += 10) {
			Mat binarized;
			adaptiveThreshold(image, binarized, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, j, 7);
			imwrite("out/block_size_calculation/gens/gen_size_" +  IntToStr(i) + "_" + IntToStr(j) + ".jpg", binarized);
		}
	}

	// COMPARING AND PRINTING THE RESULTS
	for (int i = 5000; i > 0; i -= 100) {
			Image ref_image = Image::fromFileGrayscale("out/block_size_calculation/refs/ref_size_" +  IntToStr(i) + ".jpg");
			threshold(ref_image, ref_image, 125, 255, CV_THRESH_OTSU);
			int height = (4135 / (double)5000) * i;
			int width = i;

			int cropY = (625 / (double)4135) * height;
			int cropX = (720 / (double)5000) * width;
			int cropHeight = (2860 / (double)4135) * height;
			int cropWidth = (3460 / (double)5000) * width;

			Rect roi(cropX, cropY, cropWidth, cropHeight);
			Mat ref_image_roi = ref_image(roi);
			for (int j = 3; j < 800; j += 10) {
				Image gen_image = Image::fromFileGrayscale("out/block_size_calculation/gens/gen_size_" +  IntToStr(i) + "_" + IntToStr(j) + ".jpg");
				threshold(gen_image, gen_image, 125, 255, CV_THRESH_OTSU);

				Mat gen_image_roi = gen_image(roi);
				absdiff(ref_image_roi, gen_image_roi, gen_image_roi);
				std::cout << DoubleToStr(sum(gen_image_roi).val[0]) << "\t";
			}
			std::cout << endl;
	}
    return 0;
}

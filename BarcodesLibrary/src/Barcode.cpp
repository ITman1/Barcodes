// Encoding: UTF-8

/**
 * File:      Barcode.cpp
 * Date:      March 2012
 * Project:   Barcodes Library
 * Author:    Radim Loskot
 * Brief:     XXXXXXXXXXXXXXXXXXXXXXXX @todo
 *            XXXXXXXXXXXXXXXXXXXXXXXX @todo
 */

/**
 * @file Barcode.cpp
 *
 * @brief XXXXXXXXXXXXXXXXXXXXXXXXXXXX @todo
 * @brief XXXXXXXXXXXXXXXXXXXXXXXXXXXX @todo
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

/** JUST TEST OF LIBRARY LINKING... */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

int testMELib() {
	cout << "QR Code" << endl;
    vector<int> vec;
    FastFeatureDetector detector(50);
    vector<KeyPoint> v;
    int i = 12;
    return i;
}

//============================================================================
// Name        : QRReaderCore.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

int main(void) {
	cout << "QR Code" << endl;
    vector<int> vec;
    FastFeatureDetector detector(50);
    vector<KeyPoint> v;
    int i = 12;
    return i;
}

/*
 * miscellaneous.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef MISCELLANEOUS_H_
#define MISCELLANEOUS_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

void calcBinarizedHistogram(Mat &image, Mat &contourMask, MatND &hist);
Mat getPerspectiveTransform(vector<Point> &corners, Size resultSize);
Mat warpPerspective(Mat &image, vector<Point> &corners, bool sortCorners = false, Size resultSize = Size(-1,-1));
double exactMatch(Mat &mat, Mat &tmpl, Mat &diff);
double matchTemplate(Mat img, Mat imgTemplate);
uchar rowMeanAvg(Mat &means, int row, int from, int to, int rowCuts);
void fillMissing(Mat &image, Mat &refImage, int image_EdgeDetect_MeanBlockSize, int refImage_MeanBlockSize, int image_EdgeDetect_Mean_C, int rowCuts);

} /* namespace barcodes */
#endif /* MISCELLANEOUS_H_ */

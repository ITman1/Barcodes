/*
 * miscellaneous.cpp
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "miscellaneous.h"

namespace barcodes {

double matchTemplate(Mat img, Mat imgTemplate) {
	Mat result;
	  int result_cols =  img.cols - imgTemplate.cols + 1;
	  int result_rows = img.rows - imgTemplate.rows + 1;

	  result.create( result_cols, result_rows, CV_32FC1 );

	  /// Do the Matching and Normalize
	  cv::matchTemplate( img, imgTemplate, result, CV_TM_SQDIFF );
	  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	  /// Localizing the best match with minMaxLoc
	  double minVal; double maxVal; Point minLoc; Point maxLoc;
	  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	  Mat img_display;
	  img.copyTo( img_display );

	  /// Show me what you got
	  rectangle( img_display, minLoc, Point( minLoc.x + imgTemplate.cols , minLoc.y + imgTemplate.rows ), Scalar::all(0), 2, 8, 0 );
	  rectangle( result, minLoc, Point( minLoc.x + imgTemplate.cols , minLoc.y + imgTemplate.rows ), Scalar::all(0), 2, 8, 0 );

	  return minVal;
}




Mat getPerspectiveTransform(vector<Point> &corners, Size resultSize) {
	Point2f srcPoints[4], dstPoints[4];

	srcPoints[0].x = corners.at(3).x;
	srcPoints[0].y = corners.at(3).y;
	srcPoints[1].x = corners.at(0).x;
	srcPoints[1].y = corners.at(0).y;
	srcPoints[2].x = corners.at(2).x;
	srcPoints[2].y = corners.at(2).y;
	srcPoints[3].x = corners.at(1).x;
	srcPoints[3].y = corners.at(1).y;
	dstPoints[0].x = 0;
	dstPoints[0].y = 0;
	dstPoints[1].x = resultSize.width;
	dstPoints[1].y = 0;
	dstPoints[2].x = 0;
	dstPoints[2].y = resultSize.height;
	dstPoints[3].x = resultSize.width;
	dstPoints[3].y = resultSize.height;

	return cv::getPerspectiveTransform(srcPoints, dstPoints);
}

Mat warpPerspective(Mat &image, vector<Point> &corners, bool sortCorners, Size resultSize) {
	Mat transformation;
	Mat dstImage;

	if (corners.size() > 3) {
		vector<Point> sortedCorners;
		if (sortCorners) {
			convexHull(Mat(corners), sortedCorners);
		} else {
			sortedCorners = corners;
		}
		if (resultSize.height == -1 || resultSize.width == -1) {
			resultSize = Size(image.cols, image.rows);
		}
		if (sortedCorners.size() > 3) {

			transformation = getPerspectiveTransform(sortedCorners, resultSize);
			cv::warpPerspective(image, dstImage, transformation, resultSize);

			return dstImage;
		}
	}

	dstImage.release();
	return dstImage;
}

double exactMatch(Mat &mat, Mat &tmpl, Mat &diff) {
    cv::compare(mat, tmpl, diff, cv::CMP_NE);
    return countNonZero(diff) / (double)(mat.rows * mat.cols);
}

void calcBinarizedHistogram(Mat &image, Mat &contourMask, MatND &hist) {
	// Initialization histogram arguments
	int channels[] = {0};
	int histSize[] = {2};
	float range[] = {0, 256};
	const float* ranges[] = {range};

	calcHist(&image, 1, channels, contourMask, hist, 1, histSize, ranges);
}

uchar rowMeanAvg(Mat &means, int row, int from, int to, int rowCuts) {
	if (row >= means.rows || from >= means.cols || to >= means.cols || from > to || from < 0 || rowCuts < 0) {
		return 0;
	}

	int col, sum = 0;
	for (int i = 1; i < rowCuts + 1; i++) {
		col = from + ((to - from) / (double)(rowCuts + 1)) * i;
		sum +=  means.at<uchar>(row, col);
	}

	return sum / rowCuts;

}

void fillMissing(Mat &image, Mat &refImage, int image_EdgeDetect_MeanBlockSize, int refImage_MeanBlockSize, int image_EdgeDetect_Mean_C, int rowCuts) {
	int col;
	int row;
	int cols = image.cols;
	int rows = image.rows;
	Mat mean;
	Mat ref_mean;

    boxFilter( image, mean, image.type(), Size(image_EdgeDetect_MeanBlockSize, image_EdgeDetect_MeanBlockSize),
               Point(-1,-1), true, BORDER_REPLICATE );

    boxFilter( refImage, ref_mean, refImage.type(), Size(refImage_MeanBlockSize, refImage_MeanBlockSize),
               Point(-1,-1), true, BORDER_REPLICATE );

	for( row = 0; row < rows; row++ ) {
	    uchar *icol = image.ptr<uchar>(row);
	    uchar *mcol = mean.ptr<uchar>(row);

	    bool isBackground = *mcol >= 128 - image_EdgeDetect_Mean_C;
	    int bgBegin = -1;
	    int bgEnd = -1;

	    for(col = 0 ; col < cols; icol++, mcol++, col++ ) {
	    	if (*mcol < 128 - image_EdgeDetect_Mean_C && isBackground) {
	    		bgEnd = col;
	    	} else if (*mcol >= 128 - image_EdgeDetect_Mean_C && !isBackground) {
	    		if (bgBegin != -1) {
					uchar bgMeanAvgCenter = rowMeanAvg(ref_mean, row, bgBegin, bgEnd, rowCuts);
					uchar fillMeanAvgCenter = rowMeanAvg(ref_mean, row, bgEnd, col, rowCuts);
					if (bgMeanAvgCenter < fillMeanAvgCenter + 3) {
						line(image, Point_<int>(bgBegin, row), Point_<int>(bgEnd, row), 0, 1, 8, 0);
						//line(refImage, Point_<int>(bgCenter.x, row), Point_<int>(bgCenter.x, row), CV_RGB(0, 255, 0), 2, 8, 0);
						//line(refImage, Point_<int>(fillCenter.x, row), Point_<int>(fillCenter.x, row), CV_RGB(255, 255, 0), 2, 8, 0);
					}

	    		}
	    		bgBegin = col;
	    	}

	    	isBackground = *mcol >= 128 - image_EdgeDetect_Mean_C;
	    }
	}
}

} /* namespace barcodes */

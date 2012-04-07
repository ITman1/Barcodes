///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       miscellaneous.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines some miscellaneous functions which does not belong
//             to any class. Especially functions for image transformation.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file miscellaneous.cpp
 *
 * @brief Defines some miscellaneous functions which does not belong
 *        to any class. Especially functions for image transformation.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "miscellaneous.h"

namespace barcodes {

/**
 * Finds match inside image. (not tested, never used)
 * Copied sample from: http://opencv.itseez.com/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
 *
 * @param img ...
 * @param imgTemplate ...
 * @return ...
 * @todo Finish implementation.
 */
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

/**
 * Returns transformation matrix for perspective transformation.
 *
 * @param corners Four points of the perspective projection, should be ordered from top right and clockwise.
 * @param resultSize Result size for transformation.
 * @return Transformation matrix for perspective transformation.
 */
Mat getPerspectiveTransform(vector<Point> &corners, Size resultSize) {
	Point2f srcPoints[4], dstPoints[4];

	// Coordinates in the perspective
	srcPoints[0].x = corners.at(3).x; // left up
	srcPoints[0].y = corners.at(3).y;
	srcPoints[1].x = corners.at(0).x; // right up
	srcPoints[1].y = corners.at(0).y;
	srcPoints[2].x = corners.at(2).x; // left bottom
	srcPoints[2].y = corners.at(2).y;
	srcPoints[3].x = corners.at(1).x; // right bottom
	srcPoints[3].y = corners.at(1).y;

	// Output warped coordinates
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

/**
 * Applies perspective transformation.
 *
 * @param image Image to be transformed.
 * @param corners Four points of the perspective projection, should be
 *                ordered from top right and clockwise.
 * @param sortCorners If points are not ordered, it can be done by
 *        specifying true here. The result image but miggt be rotated then.
 * @param resultSize Result size for transformation.
 * @return Warped image.
 */
Mat warpPerspective(Mat &image, vector<Point> &corners, bool sortCorners, Size resultSize) {
	Mat transformation;
	Mat dstImage;

	if (corners.size() > 3) {

		// Sorting corners if needed.
		vector<Point> sortedCorners;
		if (sortCorners) {
			convexHull(Mat(corners), sortedCorners);
		} else {
			sortedCorners = corners;
		}

		// No result size demanded, so it will be same as input image.
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

/**
 * Matches two binarized images for exact match.
 *
 * @param mat Matrix against which should be done template match.
 * @param tmpl Template used for matching.
 * @param diff Difference matrix.
 * @return The percentages in which are the same.
 */
double exactMatch(Mat &mat, Mat &tmpl, Mat &diff) {
    cv::compare(mat, tmpl, diff, cv::CMP_NE);
    return countNonZero(diff) / (double)(mat.rows * mat.cols);
}

/**
 * Calculates histogram for image which is binarized to values 0 and 255.
 *
 * @param image Binarized image.
 * @param contourMask Mask of the range from which should be calculated histogram.
 * @param hist Calculated histogram.
 */
void calcBinarizedHistogram(Mat &image, Mat &contourMask, MatND &hist) {
	// Initialization histogram arguments
	int channels[] = {0};
	int histSize[] = {2};
	float range[] = {0, 256};
	const float* ranges[] = {range};

	calcHist(&image, 1, channels, contourMask, hist, 1, histSize, ranges);
}

/**
 * Calculates average mean on the row in specified column ranges.
 * Average is determined from samples of the row. Number of
 * samples is equal to rowCuts.
 *
 * @param means The matrix of already calculated means.
 * @param row Row on which should be determined average mean.
 * @param from Column where sampling starts.
 * @param to Column where sampling ends.
 * @param rowCuts The number of samples which from which is calculated average mean.
 * @return Average mean.
 */
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

/**
 * Repairs blank places inside image which might occur after adaptive threshold.
 *
 * @param image Image after adaptive threshold.
 * @param refImage Reference image used for repair. It is image before adaptive threshold.
 * @param image_EdgeDetect_MeanBlockSize Block size of the adaptive threshold which
 *        will be used for determining edges (places where changes 0 to 1 or 1 to 0)
 * @param refImage_MeanBlockSize The block size that is used for calculation of means from reference image.
 * @param image_EdgeDetect_Mean_C The transition tolerance for edges (0 to 1 or 1 to 0).
 * @param rowCuts The number of samples which from which is calculated average mean on the row.
 *
 * @see rowMeanAvg
 */
void fillMissing(Mat &image, Mat &refImage, int image_EdgeDetect_MeanBlockSize, int refImage_MeanBlockSize, int image_EdgeDetect_Mean_C, int rowCuts) {
	int col;
	int row;
	int cols = image.cols;
	int rows = image.rows;
	Mat mean;
	Mat ref_mean;

	// Calculating the means for edge detect
    boxFilter( image, mean, image.type(), Size(image_EdgeDetect_MeanBlockSize, image_EdgeDetect_MeanBlockSize),
               Point(-1,-1), true, BORDER_REPLICATE );

    // Calculating means for fill and background determination
    boxFilter( refImage, ref_mean, refImage.type(), Size(refImage_MeanBlockSize, refImage_MeanBlockSize),
               Point(-1,-1), true, BORDER_REPLICATE );

	for( row = 0; row < rows; row++ ) {
	    uchar *icol = image.ptr<uchar>(row);
	    uchar *mcol = mean.ptr<uchar>(row);

	    // As background are supposed to be values near to the 255
	    bool isBackground = *mcol >= 128 - image_EdgeDetect_Mean_C;
	    int bgBegin = -1;
	    int bgEnd = -1;

	    for(col = 0 ; col < cols; icol++, mcol++, col++ ) {
	    	// Background ends here - 255 => 0
	    	if (*mcol < 128 - image_EdgeDetect_Mean_C && isBackground) {
	    		bgEnd = col;
	    	// Fill ends here - 0 => 255
	    	} else if (*mcol >= 128 - image_EdgeDetect_Mean_C && !isBackground) {
	    		if (bgBegin != -1) { // For filling area we need to know start of the background
					uchar bgMeanAvgCenter = rowMeanAvg(ref_mean, row, bgBegin, bgEnd, rowCuts);
					uchar fillMeanAvgCenter = rowMeanAvg(ref_mean, row, bgEnd, col, rowCuts);

					if (bgMeanAvgCenter < fillMeanAvgCenter + refImage_MeanBlockSize) {
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

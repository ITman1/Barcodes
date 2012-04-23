///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       miscellaneous.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Declares some miscellaneous functions which does not belong
//             to any class. Especially functions for image transformation.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file miscellaneous.h
 *
 * @brief Declares some miscellaneous functions which does not belong
 *        to any class. Especially functions for image transformation.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef MISCELLANEOUS_H_
#define MISCELLANEOUS_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

/**
 * Calculates histogram for image which is binarized to values 0 and 255.
 *
 * @param image Binarized image.
 * @param contourMask Mask of the range from which should be calculated histogram.
 * @param hist Calculated histogram.
 */
void calcBinarizedHistogram(Mat &image, Mat &contourMask, MatND &hist);

/**
 * Returns transformation matrix for perspective transformation.
 *
 * @param corners Four points of the perspective projection, should be ordered from top right and clockwise.
 * @param resultSize Result size for transformation.
 * @return Transformation matrix for perspective transformation.
 */
Mat getPerspectiveTransform(vector<Point> &corners, Size resultSize);

/**
 * Returns transformation matrix for perspective transformation.
 *
 * @param srcCorners Four points of the source projection, should be ordered from top right and clockwise.
 * @param dstCorners Four points of the destination projection, should be ordered from top right and clockwise.
 * @return Transformation matrix for perspective transformation.
 */
Mat getPerspectiveTransform(vector<Point> &srcCorners, vector<Point> &dstCorners);

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
Mat warpPerspective(Mat &image, vector<Point> &corners, bool sortCorners = false, Size resultSize = Size(-1,-1));

/**
 * Matches two binarized images for exact match.
 *
 * @param mat Matrix against which should be done template match.
 * @param tmpl Template used for matching.
 * @param diff Difference matrix.
 * @return The percentages in which are the same.
 */
double exactMatch(Mat &mat, Mat &tmpl, Mat &diff);

/**
 * Finds match inside an image.
 *
 * @param img Source image.
 * @param imgTemplate Template image.
 * @param matchLoc Location with the best match.
 * @return Value of the match.
 *
 * @see http://opencv.itseez.com/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
 */
double matchTemplate(Mat img, Mat imgTemplate, int match_method, Point &matchLoc);

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
uchar rowMeanAvg(Mat &means, int row, int from, int to, int rowCuts);

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
void fillMissing(Mat &image, Mat &refImage, int image_EdgeDetect_MeanBlockSize, int refImage_MeanBlockSize, int image_EdgeDetect_Mean_C, int rowCuts);

} /* namespace barcodes */
#endif /* MISCELLANEOUS_H_ */

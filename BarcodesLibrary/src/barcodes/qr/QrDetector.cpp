/*
 * QrDetector.cpp
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#include <opencv2/imgproc/imgproc.hpp>

#include "../../common.h"
#include "QrDetector.h"
#include "QrBuildHelper.h"

#define DEBUG_TAG "QrDetector.cpp"

namespace barcodes {
void QrDetector::detect(Image &image, DetectedMarks &detectedMarks, int flags) const {
	detectedMarks.clear();
	DEBUG_PRINT(DEBUG_TAG, "================ NEW IMAGE DETECT CALL ================");
	if (image.data != NULL) {
		DetectedMarks marks;
		int repairFlags = flags & REPAIR_FLAGS;

		flags = flags & ~repairFlags;

		detectByDistancePriority(image, detectedMarks, flags);
		if (detectedMarks.size() > 2) return;

		if (repairFlags & FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR) {
			detectByDistancePriority(image, marks, flags | FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR);
			detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
			detectedMarks.filter(QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
			if (detectedMarks.size() > 2) return;
		}

		if (repairFlags & FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR) {
			detectByDistancePriority(image, marks, flags | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR);
			detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
			detectedMarks.filter(QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
			if (detectedMarks.size() > 2) return;
		}
	}
}



void QrDetector::detectByDistancePriority(Image &image, DetectedMarks &detectedMarks, int flags) const {
DEBUG_PRINT(DEBUG_TAG, "detectByDistancePriority(image,detectedMarks,%d)", flags);

	DetectedMarks marks;
	Mat binarized;
	int distanceFlags = flags & DISTANCE_FLAGS;
	flags = flags & ~distanceFlags;

	// Default decoding
	if (distanceFlags == 0) {
		binarized = binarize(image, flags);
		_detect(binarized, marks, flags);
	}

	// Near distance
	if (distanceFlags & FLAG_DISTANCE_NEAR) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_NEAR);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		if (detectedMarks.size() > 2) return;
	}

	// Medium distance
	if (distanceFlags & FLAG_DISTANCE_MEDIUM) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_MEDIUM);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		detectedMarks.filter(QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
		if (detectedMarks.size() > 2) return;
	}

	// Far distance
	if (distanceFlags & FLAG_DISTANCE_FAR) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_FAR);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		detectedMarks.filter(QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
		if (detectedMarks.size() > 2) return;
	}

	// Far Far distance
	if (distanceFlags & FLAG_DISTANCE_FAR_FAR) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_FAR_FAR);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		detectedMarks.filter(QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
	}
}


void QrDetector::_detect(Mat &image, DetectedMarks &detectedMarks, int flags) const {
DEBUG_PRINT(DEBUG_TAG, "_detect(image,detectedMarks,%d)", flags);

	vector<vector<Point> > drawVec;
	Mat cropped, diff;
	vector<vector<Point> > contours;
	DetectedMark currMark;
	Mat contourImage = image.clone();
	Mat qrMark = QrBuildHelper::buildQrMark(QR_MARK_TEMPLATE_SIZE);
	detectedMarks.clear();

	double matchTolerance = QR_MARK_TAMPLATE_MATCH_TOLERANCE_NORMAL;
	int markMinSize = QR_MARK_MINIMAL_SIZE_NORMAL;
	if (flags & FLAG_QR_MARK_MATCH_TOLERANCE_HIGH) {
		matchTolerance = QR_MARK_TAMPLATE_MATCH_TOLERANCE_HIGH;
		markMinSize = QR_MARK_MINIMAL_SIZE_SMALL;
	} else if (flags & FLAG_QR_MARK_MATCH_TOLERANCE_LOW) {
		matchTolerance = QR_MARK_TAMPLATE_MATCH_TOLERANCE_LOW;
		markMinSize = QR_MARK_MINIMAL_SIZE_LARGE;
	}

	DEBUG_PRINT(DEBUG_TAG, "================ NEW DETECT CALL ================ ");
	findContours(contourImage, contours, CV_RETR_LIST , CV_CHAIN_APPROX_SIMPLE);

	for( unsigned int i = 0; i < contours.size(); i++ ) {
		if (contours[i].size() < 4) continue;

		//approxPolyDP(Mat(contours[i]), contours[i], 7, true);

		RotatedRect box = minAreaRect(Mat(contours[i]));
		Size2f boxSize = box.size;
		Rect boxRect = box.boundingRect();
		Size2f boxRectSize = boxRect.size();
		Point boxCenter = box.center;

		if ((boxSize.width / (double)boxSize.height > QR_MARK_BOUNDING_RECT_MAX_ACCEPTED_SCALE)
			|| (boxSize.height / (double)boxSize.width > QR_MARK_BOUNDING_RECT_MAX_ACCEPTED_SCALE)
			|| (boxSize.width < markMinSize) ||(boxSize.height < markMinSize)
			|| (image.cols /(double) boxSize.width < QR_MARK_MAXIMAL_SIZE_RATIO)
			|| (image.rows /(double) boxSize.height < QR_MARK_MAXIMAL_SIZE_RATIO))
			continue;

		MatND hist;

		// Getting the hull mask and cropping the contour image
		Mat contourMask = Mat::zeros(boxRect.size(), CV_8UC1);
		drawVec.push_back(contours[i]);
	    drawContours(contourMask, contours, i, Scalar(255), CV_FILLED, 8, noArray(), INT_MAX, Point(-boxRect.x, -boxRect.y));
		getRectSubPix(image, boxRect.size(), box.center, cropped);

		calcBinarizedHistogram(cropped, contourMask, hist);
        float fill_density = hist.at<float>(0);
        float bg_density = hist.at<float>(1);

		if (fill_density < 1) continue;
        double bgFillRatio = (fill_density + bg_density) / (double)bg_density;
        DEBUG_PRINT(DEBUG_TAG, "hist match: %d : %.4f", i, bgFillRatio);
        if ((bgFillRatio > QR_MARK_BG_FILL_RATIO_MAX) || (bgFillRatio < QR_MARK_BG_FILL_RATIO_MIN)) continue;

		// Converting the approx to the hull
		vector<Point> hull;
		convexHull(Mat(contours[i]), hull);
		Mat hullMask = Mat::zeros(boxRect.size(), CV_8UC1);
		drawVec.clear(); drawVec.push_back(hull);
	    drawContours(hullMask, drawVec, -1, Scalar(255), CV_FILLED, 8, noArray(), INT_MAX, Point(-boxRect.x, -boxRect.y));
	    int contourFill = cv::countNonZero(contourMask);
	    int hullFill = cv::countNonZero(hullMask);
        DEBUG_PRINT(DEBUG_TAG, "cont/hull compare: %d : %d : %d : %d", i, contourFill, hullFill, boxRectSize.width * boxRectSize.height);
	    if (contourFill / (double) hullFill < QR_MARK_CONVEX_CONTOUR_MATCH) continue;

		int pointSize = ceil((boxSize.width + boxSize.height) / 7.0); // divide 7 (as points), then 2 (as the width/height ratio) and multiply * 2 (gives better result in smaller images)
		vector<Point> corners;
		Polygon2D::findCorners(hull, 4, pointSize, corners, QR_MARK_MINIMAL_CORNER_ANGLE, QR_MARK_OPTIMAL_CORNER_ANGLE);
        DEBUG_PRINT(DEBUG_TAG, "corners: %d", corners.size());
		if (corners.size() != 4) continue;

		vector<Point> offsetCorners = corners;
		Polygon2D::offset(offsetCorners, Point(-boxRect.x, -boxRect.y));
		cropped = warpPerspective(cropped, offsetCorners, true);
		if (cropped.data == NULL) continue;

		resize(cropped, cropped, Size(QR_MARK_TEMPLATE_SIZE, QR_MARK_TEMPLATE_SIZE));
	    threshold(cropped, cropped, GLOBAL_THRESH, 255, CV_THRESH_OTSU);

	    double match = exactMatch(cropped, qrMark, diff);
        DEBUG_PRINT(DEBUG_TAG, "First match result: %.4f", match);

	    currMark.flags = 0;
	    if (match > matchTolerance) {
	    	if (!(flags & FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR)) {
	    		continue;
	    	}
	    	floodFill(cropped, Point(1, 1), Scalar(0));
	    	floodFill(cropped, Point(1, cropped.rows - 2), Scalar(0));
	    	floodFill(cropped, Point(cropped.cols - 2, 1), Scalar(0));
	    	floodFill(cropped, Point(cropped.cols - 2, cropped.rows - 2), Scalar(0));

		    match = exactMatch(cropped, qrMark, diff);
	    	if (match > matchTolerance) continue;
	    	currMark.flags = FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR;
	    }
        DEBUG_PRINT(DEBUG_TAG, "mark match: %d : %.4f", i, match);

	    convexHull(corners, corners);
	    currMark.match = match;
	    currMark.flags = flags & DISTANCE_FLAGS;
	    currMark.points = corners;
	    detectedMarks.push_back(currMark);
	}
}

int QrDetector::getBlockSize(Size imageSize, double distanceDivider) {
	int max = (imageSize.width > imageSize.height)? imageSize.width : imageSize.height;
	int ret = max * MEAN_BLOCK_SIZE_PER_SIZE_COEFFICIENT / distanceDivider;
	return (ret < 3)? 3 : ret + (ret + 1) % 2;
}

Mat QrDetector::binarize(Image &image, int flags, int mean_C) {
	Mat binarized;

	if (flags & FLAG_GLOBAL_THRESH) {
		threshold(image, binarized, GLOBAL_THRESH, 255, CV_THRESH_OTSU);
	} else {
		int blockSize;
		if (flags & FLAG_DISTANCE_NEAR) {
			blockSize = getBlockSize(Size(image.cols, image.rows), DISTANCE_NEAR_MEAN_BLOCK_SIZE_DIVIDER);
		} else if (flags & FLAG_DISTANCE_FAR) {
			blockSize = getBlockSize(Size(image.cols, image.rows), DISTANCE_FAR_MEAN_BLOCK_SIZE_DIVIDER);
		} else {
			blockSize = getBlockSize(Size(image.cols, image.rows), DISTANCE_MEDIUM_MEAN_BLOCK_SIZE_DIVIDER);
		}

		adaptiveThreshold(image, binarized, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, blockSize, mean_C);
		if (flags & FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR) {
			fillMissing(binarized, image, EDGE_DETECT_MEAN_BLOCK_SIZE, MEAN_BLOCK_SIZE, EDGE_DETECT_MEAN_C, ROW_CUTS);
		}
	}

	//DEBUG_WRITE_IMAGE("after_test_binarized.jpg", binarized);
	return binarized;
}
} /* namespace barcodes */

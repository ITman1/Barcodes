///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       DetectedMarks.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Implements member methods of DetectedMarks class which is returned
//             by detect methods and contains detected localization marks.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file DetectedMarks.cpp
 *
 * @brief Implements member methods of DetectedMarks class which is returned
 *        by detect methods and contains detected localization marks.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <opencv2/imgproc/imgproc.hpp>

#include "DetectedMarks.h"
#include "../common/Vector2D.h"

namespace barcodes {

/**
 * Filters detected marks which too close.
 *
 * @param centerPointsMinDistance Minimal possible allowed distance ratio between centers of the marks.
 *        Minimal distance is calculated as a product of multiplying sizes of bounding rectangle and this ratio.
 */
void DetectedMarks::filter(double centerPointsMinDistance) {

	vector<DetectedMark>::iterator iter, iter2;

	for (iter = this->begin(); iter != this->end(); iter++) {
		// Centers are calculated as a centers of bounding rectangles.
		RotatedRect refMarkBox = minAreaRect(Mat(iter->points));
		for (iter2 = this->begin(); iter2 != this->end(); ) {
			RotatedRect markBox = minAreaRect(Mat(iter2->points));
			Vector2D centerDiff(refMarkBox.center, markBox.center);

			if ((iter != iter2)
				&& (fabs(centerDiff.dx) / (double)refMarkBox.boundingRect().width < centerPointsMinDistance)
				&& (fabs(centerDiff.dy) / (double)refMarkBox.boundingRect().height < centerPointsMinDistance)
			)  { // This mark is too close, filter it
					iter2 = this->erase(iter2);
					iter = this->begin();
			} else {
				iter2++;
			}
		}
	}
}

/**
 * Perspective transform of the marks.
 *
 * @param transformation Transformation matrix.
 */
void DetectedMarks::perspectiveTransform(Mat &transformation) {
	vector<DetectedMark>::iterator iter;
	Mat res;

	for (iter = this->begin(); iter != this->end(); iter++) {
		vector<Point2f> points2f;
		vector<Point>::iterator iter2i;

		for (iter2i = iter->points.begin(); iter2i != iter->points.end(); iter2i++) {
			points2f.push_back(*iter2i);
		}

		cv::perspectiveTransform(Mat(points2f), res, transformation);

		iter->points.clear();
		for (unsigned int i = 0; i < points2f.size(); i++) {
			iter->points.push_back(res.at<Point2f>(i, 0));
		}
	}
}

}

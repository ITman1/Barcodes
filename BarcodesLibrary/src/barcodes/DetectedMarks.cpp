/*
 * DetectedMark.cpp
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#include <opencv2/imgproc/imgproc.hpp>

#include "DetectedMarks.h"
#include "../common/Vector2D.h"

namespace barcodes {
void DetectedMarks::filter(double centerPointsMinDistance) {

	vector<DetectedMark>::iterator iter, iter2;

	for (iter = this->begin(); iter != this->end(); iter++) {
		RotatedRect refMarkBox = minAreaRect(Mat(iter->points));
		for (iter2 = this->begin(); iter2 != this->end(); ) {
			RotatedRect markBox = minAreaRect(Mat(iter2->points));
			Vector2D centerDiff(refMarkBox.center, markBox.center);

			if ((iter != iter2)
				&& (fabs(centerDiff.dx) / (double)refMarkBox.boundingRect().width < centerPointsMinDistance)
				&& (fabs(centerDiff.dy) / (double)refMarkBox.boundingRect().height < centerPointsMinDistance)
			)  {
				if (iter2 == iter + 1) {
					iter2 = this->erase(iter2);
					iter = this->begin();
				} else {
					iter2 = this->erase(iter2);
				}
			} else {
				iter2++;
			}
		}
	}
}

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
		for (int i = 0; i < points2f.size(); i++) {
			iter->points.push_back(res.at<Point2f>(i, 0));
		}
	}
}

}

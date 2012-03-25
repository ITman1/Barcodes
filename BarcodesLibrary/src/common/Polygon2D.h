/*
 * Polygon2D.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef POLYGON2D_H_
#define POLYGON2D_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Line2D.h"

namespace barcodes {
using namespace cv;

struct PointNearbyPoints {
	Point point;
	vector<Point> nearbyLeftPoints;
	vector<Point> nearbyRightPoints;
};

template<typename T>
class Polygon2D_ {
protected:
	static void getNearbyPoints(vector<Point> &polygonPoints, vector<PointNearbyPoints> &pointNearbyPoints, vector<Point> &cornerArea) {
		vector<Point> _cornerArea;

		int pointsSize = polygonPoints.size();

		if (pointsSize > 2) {

			for (int i = 0; i < pointsSize; i++) {
				PointNearbyPoints currNearbyPoints;
				currNearbyPoints.point = polygonPoints[i];
				_cornerArea = cornerArea;
				offset(_cornerArea, polygonPoints[i]);

				bool isRight = true;
				Point lastPoint;
				bool hasLeftOuterPoint = false;
				for (int j = (i + 1) % pointsSize; j != i; j = (j + 1) % pointsSize) {
					// This point lies inside conrner area
					if (pointPolygonTest(_cornerArea, polygonPoints[j], false) > 0) {
						if (isRight) { // This is inner right
							currNearbyPoints.nearbyRightPoints.push_back(polygonPoints[j]);
						} else { // This inner, but already left
							if (!hasLeftOuterPoint) { // Insert the outer left if not set
								hasLeftOuterPoint = true;
								currNearbyPoints.nearbyLeftPoints.push_back(lastPoint);
							}
							currNearbyPoints.nearbyLeftPoints.push_back(polygonPoints[j]);
						}
					} else { // Point lies outside the corner area

						// There is no more right inner points, insert the outer right point
						if (isRight) currNearbyPoints.nearbyRightPoints.push_back(polygonPoints[j]);

						// This is the last point and the left outer is not still set, insert it
						if (((j + 1) % pointsSize == i) && (!hasLeftOuterPoint)) {
							hasLeftOuterPoint = true;
							currNearbyPoints.nearbyLeftPoints.push_back(lastPoint);
						}

						isRight = false; // No more right points will be
					}

					lastPoint = polygonPoints[j];
				}
				pointNearbyPoints.push_back(currNearbyPoints);
			}
		}
	}
public:
	vector<Point> &polygonPoints;

	Polygon2D_() {}
	virtual ~Polygon2D_() {}

	static void filterPointsInArea(vector<Point> &polygonPoints, vector<Point> &area)  {
		vector<Point> _area;
		vector<Point>::iterator iter, iter2;
		vector<Point> inArea;
		Point avgPoint, currPoint;

		bool noChange = false;
		while (!noChange) {
			noChange = true;
			for (iter = polygonPoints.begin(); iter != polygonPoints.end(); iter++) {
				currPoint = *iter;
				_area = area;
				offset(_area, currPoint);
				inArea.clear();

				iter = polygonPoints.erase(iter);
				for (iter2 = polygonPoints.begin(); iter2 != polygonPoints.end(); ) {
					if (pointPolygonTest(_area, *iter2, false) > 0) {
						inArea.push_back(*iter2);
						iter2 = polygonPoints.erase(iter2);
					} else {
						iter2++;
					}
				}

				if (inArea.size() > 0) {
					avgPoint = Point(0, 0);
					inArea.push_back(currPoint);
					for (iter2 = inArea.begin(); iter2 != inArea.end(); iter2++) {
						avgPoint.x += iter2->x;
						avgPoint.y += iter2->y;
					}
					avgPoint.x = avgPoint.x / (double) inArea.size();
					avgPoint.y = avgPoint.y / (double) inArea.size();
					polygonPoints.push_back(avgPoint);
					noChange = false;
					break;
				} else {
					polygonPoints.push_back(currPoint);
				}
			}
		}
	}

	static void findCorners(vector<Point> &polygonPoints, int cornerCount, int minCornerArea, vector<Point> &cornerPoints, double minCornerAngle, double optimalAngle) {

		vector<PointNearbyPoints>::iterator nearbyPointsIter;
		vector<PointNearbyPoints> pointNearbyPoints;
		vector<Point> cornerArea;
		cornerArea.push_back(Point(-minCornerArea, -minCornerArea));
		cornerArea.push_back(Point(minCornerArea, -minCornerArea));
		cornerArea.push_back(Point(minCornerArea, minCornerArea));
		cornerArea.push_back(Point(-minCornerArea, minCornerArea));

		cornerPoints.clear();
		if (cornerCount > 2) {

			getNearbyPoints(polygonPoints, pointNearbyPoints, cornerArea);

			for (nearbyPointsIter = pointNearbyPoints.begin(); nearbyPointsIter != pointNearbyPoints.end(); nearbyPointsIter++) {
				Point leftInnerPoint, rightInnerPoint;

				vector<Point> *nearbyLeftPoints = &(*nearbyPointsIter).nearbyLeftPoints;
				if ((*nearbyPointsIter).nearbyLeftPoints.size() > 1) {
					leftInnerPoint = (*nearbyLeftPoints)[1];
				} else {
					leftInnerPoint = (*nearbyPointsIter).point;
				}

				vector<Point> *nearbyRightPoints = &(*nearbyPointsIter).nearbyRightPoints;
				if (nearbyRightPoints->size() > 1) {
					rightInnerPoint = (*nearbyRightPoints)[nearbyRightPoints->size() - 2];
				} else {
					rightInnerPoint = (*nearbyPointsIter).point;
				}

				double angle1 = fastAtan2(
						leftInnerPoint.x - nearbyLeftPoints->front().x,
						leftInnerPoint.y - nearbyLeftPoints->front().y
				);

				double angle2 = fastAtan2(
						rightInnerPoint.x - nearbyRightPoints->back().x,
						rightInnerPoint.y - nearbyRightPoints->back().y
				);

				angle1 = (angle1 > 180)? angle1 - 180 : angle1;
				angle2 = (angle2 > 180)? angle2 - 180 : angle2;

				double angleDiff = fabs(angle1 - angle2);
				if ( angleDiff > minCornerAngle && angleDiff < 180 - optimalAngle) {
					Point intersect;
					bool inter = Line2D::intersection(Line2D(leftInnerPoint, nearbyLeftPoints->front()),
							Line2D(rightInnerPoint, nearbyRightPoints->back()), intersect);
					if (inter) {
						cornerPoints.push_back(intersect);
					}
				}
			}

			filterPointsInArea(cornerPoints, cornerArea);
		}
	}

	static void rotate(vector<Point> &polygonPoints, Point2f pivot, float angle) {
		Mat rotMat = getRotationMatrix2D(pivot, angle, 1.0);
		transform(polygonPoints, polygonPoints, rotMat);
	}

	static void offset(vector<Point> &polygonPoints, Point2f offset) {
		vector<Point>::iterator iter;

		for (iter = polygonPoints.begin(); iter != polygonPoints.end(); iter++) {
			(*iter).x += offset.x;
			(*iter).y += offset.y;
		}
	}

	static int getNearestPointPosition(vector<Point> &contour, vector<Point> &points, bool preferInside = true) {
		double min = 1e308, dist;
		int position = -1;
		int length = points.size();

		if (preferInside) {
			for (int i = 0; i < length; i++) {
				dist = pointPolygonTest(Mat(contour), points[i], true);
				if (((min < 0) && (dist >= min)) || ((min >= 0) && (dist >= 0) && (dist <= min)) || (position == -1)) {
					min = dist;
					position = i;
				}
			}
		} else {
			for (int i = 0; i < length; i++) {
				dist = pointPolygonTest(Mat(contour), points[i], true);
				if (abs(dist) < min ) {
					min = abs(dist);
					position = i;
				}
			}
		}

		return position;
	}

};

typedef Polygon2D_<int> Polygon2D;
typedef Polygon2D_<float> Polygon2Df;
}
#endif /* POLYGON2D_H_ */

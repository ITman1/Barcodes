///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Polygon2D.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Polygon2D_ template which encapsulates possible
//             operations with 2D polygon defined by set of the points.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Polygon2D.h
 *
 * @brief Defines the Polygon2D_ template which encapsulates possible
 *        operations with 2D polygon defined by set of the points.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef POLYGON2D_H_
#define POLYGON2D_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Line2D.h"

namespace barcodes {
using namespace cv;

/**
 * Struct which holds information about neighbor points around some specific point.
 */
struct PointNearbyPoints {
	/**
	 * Point against which are gathered neighbor points
	 */
	Point point;
	/**
	 * Neighbor points which are located on the left from this specific point (clock-wise).
	 */
	vector<Point> nearbyLeftPoints;
	/**
	 * Neighbor points which are located on the right from this specific point (clock-wise).
	 */
	vector<Point> nearbyRightPoints;
};

/**
 * Template for creation 2D polygons which are defined as a set of points.
 *
 * @todo For now only gathers static methods which should be templated.
 */
template<typename T>
class Polygon2D_:vector<Point_<T> > {
protected:
	/**
	 * Gathers points into PointNearbyPoints structure. Into this structure are stored all
	 * neighbor points of some specific point. This structure is examined for all points
	 * of the polygon.
	 *
	 * \code
	 * Structure gathers neighbors points as follow:
	 * *   * ** ** ** *****   * |  * ** (*) ******| * * **  * ** * *** * * **
	 *                          | left   P  right |
	 * --(outside corner area)--|--(corner area)--|--(outside corner area)--
  	 * \endcode
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param pointNearbyPoints Vector containing demanded structures for all points of the polygon.
	 * @param cornerArea The area around which should be gathered neighbor points.
	 *                   The mass center should lie at (0,0). No calculation is done here.
	 *
	 * @see PointNearbyPoints
	 */

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
					// This point lies inside corner area
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

	virtual ~Polygon2D_() {}

	/**
	 * Filters points which are inside area of another point. Which results in that each
	 * point cannot contain in own area another point. The result point is calculated
	 * as an average between all points inside area.
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param area The area which should be empty around every point.
	 *             The mass center should lie at (0,0). No calculation is done here.
	 */
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
				offset(_area, currPoint);          // Offset area to current point.
				inArea.clear();

				// Erase current point from polygon, so it won't not collision
				// in later test and assumption that it is just another point inside area.
				iter = polygonPoints.erase(iter);
				for (iter2 = polygonPoints.begin(); iter2 != polygonPoints.end(); ) {
					if (pointPolygonTest(_area, *iter2, false) > 0) {
						// This point is inside area another point add this point inside vector to filter
						inArea.push_back(*iter2);
						iter2 = polygonPoints.erase(iter2); // Remove also this point from the polygon
					} else {
						iter2++;
					}
				}

				if (inArea.size() > 0) { // There are some points in area
					// Calculation of the average point
					avgPoint = Point(0, 0);
					inArea.push_back(currPoint);
					for (iter2 = inArea.begin(); iter2 != inArea.end(); iter2++) {
						avgPoint.x += iter2->x;
						avgPoint.y += iter2->y;
					}
					avgPoint.x = avgPoint.x / (double) inArea.size();
					avgPoint.y = avgPoint.y / (double) inArea.size();

					// Pushing a new current point
					polygonPoints.push_back(avgPoint);
					noChange = false;
					break;
				} else { // No points in the area of current point return this back to the polygon
					polygonPoints.push_back(currPoint);
				}
			}
		}
	}

	/**
	 * Finds corners of the polygon.
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param cornerCount The number of demanding points. (not supported)
	 * @param minCornerArea The size of the area inside which should be gathered points
	 *        and should be examined for all these points that they belongs to one corner.
	 * @param cornerPoints The result points of the corners.
	 * @param minCornerAngle The minimal angle which can have a corners.
	 * @param optimalAngle The optimal angle of the corner (not supported).
	 *
	 * @todo Selecting/filtering the result corners by optimalAngle or cornerCount.
	 */
	static void findCorners(vector<Point> &polygonPoints, int cornerCount, int minCornerArea, vector<Point> &cornerPoints, double minCornerAngle, double optimalAngle) {

		vector<PointNearbyPoints>::iterator nearbyPointsIter;
		vector<PointNearbyPoints> pointNearbyPoints;

		// Building the corner area as a rectangle with the mass center at the (0, 0)
		vector<Point> cornerArea;
		cornerArea.push_back(Point(-minCornerArea, -minCornerArea));
		cornerArea.push_back(Point(minCornerArea, -minCornerArea));
		cornerArea.push_back(Point(minCornerArea, minCornerArea));
		cornerArea.push_back(Point(-minCornerArea, minCornerArea));

		cornerPoints.clear();
		if (cornerCount > 2) {

			// Calculating the neighbor points
			getNearbyPoints(polygonPoints, pointNearbyPoints, cornerArea);

			for (nearbyPointsIter = pointNearbyPoints.begin(); nearbyPointsIter != pointNearbyPoints.end(); nearbyPointsIter++) {
				Point leftInnerPoint, rightInnerPoint;

				// Retrieving the most closest point in left to this point if exists
				vector<Point> *nearbyLeftPoints = &(*nearbyPointsIter).nearbyLeftPoints;
				if ((*nearbyPointsIter).nearbyLeftPoints.size() > 1) {
					leftInnerPoint = (*nearbyLeftPoints)[1];    // There is inner point
				} else {
					leftInnerPoint = (*nearbyPointsIter).point; // No inner point, take this point
				}

				// Retrieving the most closest point in right to this point if exists
				vector<Point> *nearbyRightPoints = &(*nearbyPointsIter).nearbyRightPoints;
				if (nearbyRightPoints->size() > 1) {
					rightInnerPoint = (*nearbyRightPoints)[nearbyRightPoints->size() - 2];
				} else {
					rightInnerPoint = (*nearbyPointsIter).point; // No inner point, take this point
				}

				// Creating two lines from most inner and outer points and calculating its angle

				// Calculating the angle of line in the left
				double angle1 = fastAtan2(
						leftInnerPoint.x - nearbyLeftPoints->front().x,
						leftInnerPoint.y - nearbyLeftPoints->front().y
				);

				// Calculating the angle of line in the right
				double angle2 = fastAtan2(
						rightInnerPoint.x - nearbyRightPoints->back().x,
						rightInnerPoint.y - nearbyRightPoints->back().y
				);

				angle1 = (angle1 > 180)? angle1 - 180 : angle1;
				angle2 = (angle2 > 180)? angle2 - 180 : angle2;

				// Angle has to be in the ranges acceptable for corners
				double angleDiff = fabs(angle1 - angle2);
				if ( angleDiff > minCornerAngle && angleDiff < 180 - minCornerAngle) {
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

	/**
	 * Rotates points of the polygon.
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param pivot Pivot point around which should be rotated.
	 * @param angle Angle about which should be rotated.
	 */
	static void rotate(vector<Point> &polygonPoints, Point2f pivot, float angle) {
		Mat rotMat = getRotationMatrix2D(pivot, angle, 1.0);
		transform(polygonPoints, polygonPoints, rotMat);
	}

	/**
	 * Offsets polygon points about some specified point.
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param offset Point about which should be offset all points of the polygon.
	 */
	static void offset(vector<Point> &polygonPoints, Point2f offset) {
		vector<Point>::iterator iter;

		for (iter = polygonPoints.begin(); iter != polygonPoints.end(); iter++) {
			(*iter).x += offset.x;
			(*iter).y += offset.y;
		}
	}

	/**
	 * Retrieves the position of the point inside vector which is nearest to specified polygon.
	 *
	 * @param polygonPoints Points of the polygon.
	 * @param points Points which from which should be revealed position of the point.
	 * @param preferInside Specifies whether should be preferred points inside polygon.
	 * @return Position of the nearest point inside point vector to polygon.
	 */
	static int getNearestPointPosition(vector<Point> &polygonPoints, vector<Point> &points, bool preferInside = true) {
		double min = 1e308, dist;
		int position = -1;
		int length = points.size();

		if (preferInside) {
			for (int i = 0; i < length; i++) {
				dist = pointPolygonTest(Mat(polygonPoints), points[i], true);
				if (((min < 0) && (dist >= min)) || ((min >= 0) && (dist >= 0) && (dist <= min)) || (position == -1)) {
					min = dist;
					position = i;
				}
			}
		} else {
			for (int i = 0; i < length; i++) {
				dist = pointPolygonTest(Mat(polygonPoints), points[i], true);
				if (abs(dist) < min ) {
					min = abs(dist);
					position = i;
				}
			}
		}

		return position;
	}

};

/**
 * Short name for Polygon2D_<int> template definition.
 */
typedef Polygon2D_<int> Polygon2D;

/**
 * Short name for Polygon2D_<float> template definition.
 */
typedef Polygon2D_<float> Polygon2Df;
}
#endif /* POLYGON2D_H_ */

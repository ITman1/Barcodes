///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersHelper.cpp
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines helper class which gathers some common methods
//             for getting the perspective corners.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersHelper.cpp
 *
 * @brief Defines helper class which gathers some common methods
 * for getting the perspective corners.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "PerspCornersHelper.h"
#include "../../../common/Line2D.h"
#include "../../../common/Polygon2D.h"

namespace barcodes {

bool PerspCornersHelper::sortDetectedMarks(DetectedMarks &detectedMarks) {

	if ((detectedMarks.size() < 3) || (detectedMarks[0].points.size() < 4)
			|| (detectedMarks[1].points.size() < 4) || (detectedMarks[2].points.size() < 4)) {
		return false;
	}

	// Sort each points of detected marks clockwise
	vector<Point> *mark1_points = &detectedMarks[0].points;
	convexHull(Mat(*mark1_points), *mark1_points);
	vector<Point> *mark2_points = &detectedMarks[1].points;
	convexHull(Mat(*mark2_points), *mark2_points);
	vector<Point> *mark3_points = &detectedMarks[2].points;
	convexHull(Mat(*mark3_points), *mark3_points);

	// Get center of each mark
	Point mark1_center;
	Line2D::intersection(Line2D((*mark1_points)[0], (*mark1_points)[2]), Line2D((*mark1_points)[1], (*mark1_points)[3]), mark1_center);
	Point mark2_center;
	Line2D::intersection(Line2D((*mark2_points)[0], (*mark2_points)[2]), Line2D((*mark2_points)[1], (*mark2_points)[3]), mark2_center);
	Point mark3_center;
	Line2D::intersection(Line2D((*mark3_points)[0], (*mark3_points)[2]), Line2D((*mark3_points)[1], (*mark3_points)[3]), mark3_center);

	// Get sorted points of the polygon made by the centers of marks
	vector<Point> mark_centers_disorted, mark_centers;
	mark_centers_disorted.push_back(mark1_center);
	mark_centers_disorted.push_back(mark2_center);
	mark_centers_disorted.push_back(mark3_center);
	convexHull(Mat(mark_centers_disorted), mark_centers);

	// Changing position inside detectedMarks due to previous sorting of the centers
	// Detected marks will be sorted clock-wise
	vector<Point>::iterator iter;
	DetectedMarks _detectedMarks;
	for (iter = mark_centers.begin(); iter != mark_centers.end(); iter++) {
		if (*iter == mark1_center) {
			_detectedMarks.push_back(detectedMarks[0]);
		} else if (*iter == mark2_center) {
			_detectedMarks.push_back(detectedMarks[1]);
		} else {
			_detectedMarks.push_back(detectedMarks[2]);
		}
	}
	detectedMarks = _detectedMarks;

	// Pointers to points has be changed too (positions has changed)
	mark1_points = &detectedMarks[0].points;
	mark2_points = &detectedMarks[1].points;
	mark3_points = &detectedMarks[2].points;

	// Getting the positions of the nearest points from the contour mark_centers, prefers points inside
	int mark1_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark1_points, true);
	int mark2_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark2_points, true);
	int mark3_nearest_point_position = Polygon2D::getNearestPointPosition(mark_centers, *mark3_points, true);

	// Getting nearest point distance from the contour mark_centers
	double mark1_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark1_points)[mark1_nearest_point_position], true);
	double mark2_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark2_points)[mark2_nearest_point_position], true);
	double mark3_nearest_point_distance = pointPolygonTest(Mat(mark_centers), (*mark3_points)[mark3_nearest_point_position], true);

	// Rotating points that the most outer point will the first
	rotate((*mark1_points).begin(), (*mark1_points).begin() + (mark1_nearest_point_position + 2) % 4, (*mark1_points).end());
	rotate((*mark2_points).begin(), (*mark2_points).begin() + (mark2_nearest_point_position + 2) % 4, (*mark2_points).end());
	rotate((*mark3_points).begin(), (*mark3_points).begin() + (mark3_nearest_point_position + 2) % 4, (*mark3_points).end());

	// Sorting the detected marks too. First mark will be now left bottom and next clock-wise.
	if ((mark1_nearest_point_distance >= mark2_nearest_point_distance) && (mark1_nearest_point_distance >= mark3_nearest_point_distance)) {
		rotate(detectedMarks.begin(),detectedMarks.begin() + 2,detectedMarks.end());
	} else if ((mark3_nearest_point_distance >= mark2_nearest_point_distance) && (mark3_nearest_point_distance >= mark1_nearest_point_distance)) {
		rotate(detectedMarks.begin(),detectedMarks.begin() + 1,detectedMarks.end());
	}

	return true;
}

} /* namespace barcodes */

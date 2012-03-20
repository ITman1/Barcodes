/*
 * QrBarcode.cpp
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */


#include <opencv2/imgproc/imgproc.hpp>

#include "../../types.h"
#include "QrBarcode.h"

static const string DEBUG_TAG = "QrBarcode.cpp";

using namespace barcodes;

void QrBarcode::detect(Image &image, vector<DetectedMark> &detectedMarks, int flags) {
	detectedMarks.clear();
	DEBUG_PRINT(DEBUG_TAG, "================ NEW IMAGE DETECT CALL ================");
	if (image.data != NULL) {
		vector<DetectedMark> marks;
		int repairFlags = flags & REPAIR_FLAGS;

		flags = flags & ~repairFlags;

		detectByDistancePriority(image, detectedMarks, flags);
		if (detectedMarks.size() > 2) return;

		if (repairFlags & FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR) {
			detectByDistancePriority(image, marks, flags | FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR);
			detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
			filterMarks(detectedMarks);
			if (detectedMarks.size() > 2) return;
		}

		if (repairFlags & FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR) {
			detectByDistancePriority(image, marks, flags | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR);
			detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
			filterMarks(detectedMarks);
			if (detectedMarks.size() > 2) return;
		}
	}
}


void QrBarcode::read(Image &image, ByteArray &data, int flags) {
	vector<DetectedMark> detectedMarks;

	data.clear();
	detect(image, detectedMarks);

	if (detectedMarks.size() == 3) {
		_read_V2_40(image, data, detectedMarks);
	}
}

void QrBarcode::_read_V2_40(Image &image, ByteArray &data, vector<DetectedMark> &detectedMarks, int flags) {
	vector<Point> corners;
	Mat binarized = binarize(image, detectedMarks[0].flags);
	getPerspectiveCorners_V1_40(binarized, detectedMarks, corners);

	Image img = Image(image.clone(), IMAGE_COLOR_GRAYSCALE);
	cvtColor(img, img, CV_GRAY2RGB);

	line( img, corners[0],corners[0], CV_RGB(255, 0, 0), 2, 8, 0);
	line( img, corners[1],corners[1], CV_RGB(0, 255, 0), 2, 8, 0);
	line( img, corners[2],corners[2], CV_RGB(0, 0, 255), 2, 8, 0);
	line( img, corners[3],corners[3], CV_RGB(255, 255, 0), 2, 8, 0);
	DEBUG_WRITE_IMAGE("out2/just_test.jpg", img);

	Mat perspWarped = warpPerspective(binarized, corners);
	DEBUG_WRITE_IMAGE("out2/warped.jpg", perspWarped);
}

// Getting the A,B,C,D points
// A,B,C are retrieved from the X,Y,Z
// Y is the point with the maximal inner distance from the polygon (triangle)
// which is made by the center points of QR marks.
//
// B******           ******C
// * *** *           * *** *
// * *** *-----------* *** *
// * *** *           * *** *
// ******Y           Z*****N
//    |            /
//    |         /
//    |       /
//    |    /
// ******X
// * *** *
// * *** *
// * *** *
// A*****M                 D
void QrBarcode::getPerspectiveCorners_V1_40(Mat &binarized, vector<DetectedMark> &detectedMarks, vector<Point> &corners) {
	corners.clear();

	//============= DETERMINING THE THREE CORNERS (A, B, C) FROM THE DETECTED MARKS

	// Sort each points of detected marks clockwise
	vector<Point> *mark1_points = &detectedMarks[0].points;
	convexHull(Mat(*mark1_points), *mark1_points);
	vector<Point> *mark2_points = &detectedMarks[1].points;
	convexHull(Mat(*mark2_points), *mark2_points);
	vector<Point> *mark3_points = &detectedMarks[2].points;
	convexHull(Mat(*mark3_points), *mark3_points);

	// Get center of each mark
	Point mark1_center;
	intersection((*mark1_points)[0], (*mark1_points)[2], (*mark1_points)[1], (*mark1_points)[3], mark1_center);
	Point mark2_center;
	intersection((*mark2_points)[0], (*mark2_points)[2], (*mark2_points)[1], (*mark2_points)[3], mark2_center);
	Point mark3_center;
	intersection((*mark3_points)[0], (*mark3_points)[2], (*mark3_points)[1], (*mark3_points)[3], mark3_center);

	// Get sorted points of the polygon made by the centers of marks
	vector<Point> mark_centers_disorted, mark_centers;
	mark_centers_disorted.push_back(mark1_center);
	mark_centers_disorted.push_back(mark2_center);
	mark_centers_disorted.push_back(mark3_center);
	convexHull(Mat(mark_centers_disorted), mark_centers);

	// Changing position inside detectedMarks due to previous sorting of the centers
	// Detected marks will be sorted clock-wise
	vector<Point>::iterator iter;
	vector<DetectedMark> _detectedMarks;
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
	int mark1_nearest_point_position = getNearestPointPosition(mark_centers, *mark1_points, true);
	int mark2_nearest_point_position = getNearestPointPosition(mark_centers, *mark2_points, true);
	int mark3_nearest_point_position = getNearestPointPosition(mark_centers, *mark3_points, true);

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

	// The outer point we will push as a result perspective points
	corners.push_back(detectedMarks[0].points[0]);
	corners.push_back(detectedMarks[1].points[0]);
	corners.push_back(detectedMarks[2].points[0]);

	//============= DETERMINING THE FROUTH UNKNOWN CORNER (D) BY RECT SAMPLING

	// Width of the sampling rectangle will be the approximate size of the one dot divided by 2
	RotatedRect box = minAreaRect(Mat(detectedMarks[0].points));
	Size2f boxSize = box.size;
	int sampleLineWidth = (boxSize.width > boxSize.height)? ceil(boxSize.width / (double)14) : ceil(boxSize.height / (double)14);

	// The point around which will be sample rectangle rotated (point M)
	Point2f rotatePoint = detectedMarks[0].points[3];

	// The start point from which sampling begins, its distance is equal to distance
	// between the "center" of the QR code and is shifted SAMPLE_RECT_START_SHIFT degrees inside the QR code
	Vector2Df diagVector(detectedMarks[0].points[0], detectedMarks[2].points[0]);
	Vector2Df sampleVector(detectedMarks[0].points[0], detectedMarks[0].points[3]);
	Vector2Df lineShift(detectedMarks[0].points[2], detectedMarks[0].points[3]);
	lineShift.resize(sampleLineWidth / 2);
	sampleVector.rotate(SAMPLE_RECT_START_ANGLE_SHIFT);
	sampleVector.resize(diagVector.size() / 2);

	sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, sampleLineWidth, true);

	// Now find the second edge
	Point2f rotatePoint2 = detectedMarks[2].points[1];
	Vector2Df sampleVector2(detectedMarks[2].points[0], detectedMarks[2].points[1]);
	lineShift = Vector2Df(detectedMarks[2].points[2], detectedMarks[2].points[1]);
	lineShift.resize(sampleLineWidth / 2);
	sampleVector2.rotate(SAMPLE_RECT_START_ANGLE_SHIFT * -1);
	sampleVector2.resize(diagVector.size() / 2);

	sampleQrCodeEdge(binarized, sampleVector2, rotatePoint2, lineShift, sampleLineWidth, false);

	// Finally finding the 4th corner from the intersection of the lines
	Point fourthCorner;
	intersection(rotatePoint, sampleVector + rotatePoint , rotatePoint2, sampleVector2 + rotatePoint2, fourthCorner);

	corners.push_back(fourthCorner);
}

bool QrBarcode::sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample) {
	double sampleAngleStep = (clockWiseSample)? SAMPLE_RECT_ANGLE_STEP : SAMPLE_RECT_ANGLE_STEP * -1;
	double angle = 0;

	while (fabs(angle) < SAMPLE_RECT_MAX_ANGLE) {

		double sampleRatio = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        if (sampleRatio < SAMPLE_RECT_EDGE_DETECT_FILL_RATIO) {
        	bool wasLeft = false;
        	Vector2Df finalSampleVector = sampleVector;
        	int finalStep = 0;

        	// Going one step back
        	sampleVector.rotate(sampleAngleStep);
        	angle -= sampleAngleStep;

        	// Final steps procedure
        	while (finalStep < SAMPLE_RECT_FINAL_STEPS_COUNT) {
    			DEBUG_PRINT(DEBUG_TAG, "FINAL STEPS");
        		sampleAngleStep = sampleAngleStep * 0.5; // Divide sample step by 2

        		sampleVector.rotate(sampleAngleStep * -0.5); // 1/4 forward (to 1/4 position)
        		double sampleRatioLeft = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		sampleVector.rotate(sampleAngleStep * -1); // 1/2 forward (to 3/4 position)
        		double sampleRatioRight = _sampleQrCodeEdge(binarized, sampleVector, rotatePoint, lineShift, lineWidth);

        		if (sampleRatioLeft < sampleRatioRight) {
        			wasLeft = true;
        			DEBUG_PRINT(DEBUG_TAG, "WAS LEFT");
            		sampleVector.rotate(sampleAngleStep * 1.5); // 3/4 back (to begin)
        		} else {
            		sampleVector.rotate(sampleAngleStep * 0.5); // 1/4 back (to center)
        		}

        		finalStep++;
        	}

        	sampleVector = (wasLeft)? finalSampleVector : sampleVector;
        	return true;
        }

        DEBUG_PRINT(DEBUG_TAG, "Before rotate: %d ; %d", sampleVector.dx, sampleVector.dy);
		sampleVector.rotate(sampleAngleStep * -1);
        DEBUG_PRINT(DEBUG_TAG, "After rotate: %d ; %d", sampleVector.dx, sampleVector.dy);
		angle += sampleAngleStep;
	}

	DEBUG_PRINT(DEBUG_TAG, "REACHED THE END");
	return false;
}

double QrBarcode::_sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) {
	MatND hist;

	// Getting the hull mask and cropping the contour image
	Mat sampleMask = Mat::zeros(Size(binarized.cols, binarized.rows), CV_8UC1);
	line(sampleMask, lineShift + rotatePoint, sampleVector + lineShift + rotatePoint, CV_RGB(255, 255, 255), lineWidth, 8, 0);

	calcBinarizedHistogram(binarized, sampleMask, hist);
	float fill_density = hist.at<float>(0);
    float bg_density = hist.at<float>(1);

    DEBUG_PRINT(DEBUG_TAG, "BG: %.4f || FILL: %.4f", bg_density, fill_density);

    return fill_density / (double)(fill_density + bg_density);
}

int QrBarcode::getNearestPointPosition(vector<Point> &contour, vector<Point> &points, bool preferInside) {
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

void QrBarcode::detectByDistancePriority(Image &image, vector<DetectedMark> &detectedMarks, int flags) {
DEBUG_PRINT(DEBUG_TAG, "detectByDistancePriority(image,detectedMarks,%d)", flags);

	vector<DetectedMark> marks;
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
		filterMarks(detectedMarks);
		if (detectedMarks.size() > 2) return;
	}

	// Far distance
	if (distanceFlags & FLAG_DISTANCE_FAR) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_FAR);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		filterMarks(detectedMarks);
		if (detectedMarks.size() > 2) return;
	}

	// Far Far distance
	if (distanceFlags & FLAG_DISTANCE_FAR_FAR) {
		marks.clear();
		binarized = binarize(image, flags | FLAG_DISTANCE_FAR_FAR);
		_detect(binarized, marks, flags);

		detectedMarks.insert(detectedMarks.end(), marks.begin(), marks.end());
		filterMarks(detectedMarks);
	}
}

void QrBarcode::filterMarks(vector<DetectedMark> &detectedMarks, double centerPointsMinDistance) {
DEBUG_PRINT(DEBUG_TAG, "filterMarks(detectedMarks,centerPointsMinDistance)");

	vector<DetectedMark>::iterator iter, iter2;

	for (iter = detectedMarks.begin(); iter != detectedMarks.end(); iter++) {
		RotatedRect refMarkBox = minAreaRect(Mat(iter->points));
		for (iter2 = detectedMarks.begin(); iter2 != detectedMarks.end(); ) {
			RotatedRect markBox = minAreaRect(Mat(iter2->points));
			Vector2D centerDiff(refMarkBox.center, markBox.center);

			if ((iter != iter2)
				&& (fabs(centerDiff.dx) / (double)refMarkBox.boundingRect().width < QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE)
				&& (fabs(centerDiff.dy) / (double)refMarkBox.boundingRect().height < QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE)
			)  {
				if (iter2 == iter + 1) {
					iter2 = detectedMarks.erase(iter2);
					iter = detectedMarks.begin();
				} else {
					iter2 = detectedMarks.erase(iter2);
				}
			} else {
				iter2++;
			}
		}
	}
}

void QrBarcode::rotateContour(vector<Point> &contour, Point2f pivot, float angle) {
	Mat rotMat = getRotationMatrix2D(pivot, angle, 1.0);
	transform(contour, contour, rotMat);
}

void QrBarcode::offsetContour(vector<Point> &contour, Point2f offset) {
	vector<Point>::iterator iter;

	for (iter = contour.begin(); iter != contour.end(); iter++) {
		(*iter).x += offset.x;
		(*iter).y += offset.y;
	}
}

Mat QrBarcode::buildQrMark() {
	int dotSize = QR_MARK_TEMPLATE_SIZE / 7;
	Mat mark = Mat::zeros(Size(QR_MARK_TEMPLATE_SIZE, QR_MARK_TEMPLATE_SIZE), CV_8UC1);

	rectangle(mark, Rect(dotSize, dotSize, 5 * dotSize, 5 * dotSize), Scalar(255), 1);
	rectangle(mark, Rect(2 * dotSize - 1, 2 * dotSize - 1, 3 * dotSize + 2, 3 * dotSize + 2), Scalar(255), 1);
	floodFill(mark, Point(dotSize + 1, dotSize + 1), Scalar(255));

	return mark;
}

double QrBarcode::matchTemplate(Mat img, Mat imgTemplate) {
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


bool QrBarcode::intersection(Point point1, Point point2, Point _point1, Point _point2, Point &intersect) {
    Point x = _point1 - point1;
    Point d1 = point2 - point1;
    Point d2 = _point2 - _point1;

    float inter = d1.x * d2.y - d1.y * d2.x;

    if (fabs(inter) < 1e-8) {
        return false;
    }

    double t1 = (x.x * d2.y - x.y * d2.x) / inter;
    intersect = point1 + d1 * t1;

    return true;
}

void QrBarcode::getNearbyPoints(vector<Point> &shapePoints, vector<PointNearbyPoints> &pointNearbyPoints, vector<Point> &cornerArea) {
	vector<Point> _cornerArea;

	int pointsSize = shapePoints.size();

	if (pointsSize > 2) {

		for (int i = 0; i < pointsSize; i++) {
			PointNearbyPoints currNearbyPoints;
			currNearbyPoints.point = shapePoints[i];
			_cornerArea = cornerArea;
			offsetContour(_cornerArea, shapePoints[i]);

			bool isRight = true;
			Point lastPoint;
			bool hasLeftOuterPoint = false;
			for (int j = (i + 1) % pointsSize; j != i; j = (j + 1) % pointsSize) {
				// This point lies inside conrner area
				if (pointPolygonTest(_cornerArea, shapePoints[j], false) > 0) {
					if (isRight) { // This is inner right
						currNearbyPoints.nearbyRightPoints.push_back(shapePoints[j]);
					} else { // This inner, but already left
						if (!hasLeftOuterPoint) { // Insert the outer left if not set
							hasLeftOuterPoint = true;
							currNearbyPoints.nearbyLeftPoints.push_back(lastPoint);
						}
						currNearbyPoints.nearbyLeftPoints.push_back(shapePoints[j]);
					}
				} else { // Point lies outside the corner area

					// There is no more right inner points, insert the outer right point
					if (isRight) currNearbyPoints.nearbyRightPoints.push_back(shapePoints[j]);

					// This is the last point and the left outer is not still set, insert it
					if (((j + 1) % pointsSize == i) && (!hasLeftOuterPoint)) {
						hasLeftOuterPoint = true;
						currNearbyPoints.nearbyLeftPoints.push_back(lastPoint);
					}

					isRight = false; // No more right points will be
				}

				lastPoint = shapePoints[j];
			}
			pointNearbyPoints.push_back(currNearbyPoints);
		}
	}
}

void QrBarcode::filterPointsInArea(vector<Point> &points, vector<Point> &area) {
	vector<Point> _area;
	vector<Point>::iterator iter, iter2;
	vector<Point> inArea;
	Point avgPoint, currPoint;

	bool noChange = false;
	while (!noChange) {
		noChange = true;
		for (iter = points.begin(); iter != points.end(); iter++) {
			currPoint = *iter;
			_area = area;
			offsetContour(_area, currPoint);
			inArea.clear();

			iter = points.erase(iter);
			for (iter2 = points.begin(); iter2 != points.end(); ) {
				if (pointPolygonTest(_area, *iter2, false) > 0) {
					inArea.push_back(*iter2);
					iter2 = points.erase(iter2);
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
				points.push_back(avgPoint);
				noChange = false;
				break;
			} else {
				points.push_back(currPoint);
			}
		}
	}
}

void QrBarcode::findCorners(vector<Point> &shapePoints, int cornerCount, int minCornerArea, vector<Point> &cornerPoints, double minCornerAngle, double optimalAngle) {

	vector<PointNearbyPoints>::iterator nearbyPointsIter;
	vector<PointNearbyPoints> pointNearbyPoints;
	vector<Point> cornerArea;
	cornerArea.push_back(Point(-minCornerArea, -minCornerArea));
	cornerArea.push_back(Point(minCornerArea, -minCornerArea));
	cornerArea.push_back(Point(minCornerArea, minCornerArea));
	cornerArea.push_back(Point(-minCornerArea, minCornerArea));

	cornerPoints.clear();
	if (cornerCount > 2) {

		getNearbyPoints(shapePoints, pointNearbyPoints, cornerArea);

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
			if ( angleDiff > QR_MARK_MINIMAL_CORNER_ANGLE && angleDiff < 180 - QR_MARK_MINIMAL_CORNER_ANGLE) {
				Point intersect;
				bool inter = intersection(leftInnerPoint, nearbyLeftPoints->front(),
						rightInnerPoint, nearbyRightPoints->back(), intersect);
				if (inter) {
					cornerPoints.push_back(intersect);
				}
			}
		}

		filterPointsInArea(cornerPoints, cornerArea);
	}
}

Mat QrBarcode::warpPerspective(Mat &image, vector<Point> &corners) {
	Point2f srcPoints[4], dstPoints[4];
	Mat transformation;
	Mat dstImage;

	if (corners.size() > 3) {
		vector<Point> sortedCorners;
		convexHull(Mat(corners), sortedCorners);
		if (sortedCorners.size() > 3) {
			srcPoints[0].x = sortedCorners.at(3).x;
			srcPoints[0].y = sortedCorners.at(3).y;
			srcPoints[1].x = sortedCorners.at(0).x;
			srcPoints[1].y = sortedCorners.at(0).y;
			srcPoints[2].x = sortedCorners.at(2).x;
			srcPoints[2].y = sortedCorners.at(2).y;
			srcPoints[3].x = sortedCorners.at(1).x;
			srcPoints[3].y = sortedCorners.at(1).y;
			dstPoints[0].x = 0;
			dstPoints[0].y = 0;
			dstPoints[1].x = image.cols;
			dstPoints[1].y = 0;
			dstPoints[2].x = 0;
			dstPoints[2].y = image.rows;
			dstPoints[3].x = image.cols;
			dstPoints[3].y = image.rows;

			transformation = cv::getPerspectiveTransform(srcPoints, dstPoints);

			cv::warpPerspective(image, dstImage, transformation, Size(image.cols, image.rows));
			return dstImage;
		}
	}

	dstImage.release();
	return dstImage;
}

double QrBarcode::exactMatch(Mat &mat, Mat &tmpl, Mat &diff) {
    cv::compare(mat, tmpl, diff, cv::CMP_NE);
    return countNonZero(diff) / (double)(mat.rows * mat.cols);
}

void QrBarcode::calcBinarizedHistogram(Mat &image, Mat &contourMask, MatND &hist) {
	// Initialization histogram arguments
	int channels[] = {0};
	int histSize[] = {2};
	float range[] = {0, 256};
	const float* ranges[] = {range};

	calcHist(&image, 1, channels, contourMask, hist, 1, histSize, ranges);
}

void QrBarcode::_detect(Mat &image, vector<DetectedMark> &detectedMarks, int flags) {
DEBUG_PRINT(DEBUG_TAG, "_detect(image,detectedMarks,%d)", flags);

	vector<vector<Point> > drawVec;
	Mat cropped, diff;
	vector<vector<Point> > contours;
	DetectedMark currMark;
	Mat contourImage = image.clone();
	Mat qrMark = buildQrMark();
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

		//if (contours[i].size() < 4) continue;

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

		int pointSize = (boxSize.width > boxSize.height)? ceil(boxSize.width / (double)7) * 2 : ceil(boxSize.height / (double)7) * 2;
		vector<Point> corners;
		findCorners(hull, 4, pointSize, corners);
        DEBUG_PRINT(DEBUG_TAG, "cont/hull compare: %d", corners.size());
		if (corners.size() != 4) continue;

		vector<Point> offsetCorners = corners;
		offsetContour(offsetCorners, Point(-boxRect.x, -boxRect.y));
		cropped = warpPerspective(cropped, offsetCorners);
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

uchar QrBarcode::rowMeanAvg(Mat &means, int row, int from, int to, int rowCuts) {
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

void QrBarcode::fillMissing(Mat &image, Mat &refImage, int image_EdgeDetect_MeanBlockSize, int refImage_MeanBlockSize, int image_EdgeDetect_Mean_C) {
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
					uchar bgMeanAvgCenter = rowMeanAvg(ref_mean, row, bgBegin, bgEnd);
					uchar fillMeanAvgCenter = rowMeanAvg(ref_mean, row, bgEnd, col);
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

int QrBarcode::getBlockSize(Size imageSize, double distanceDivider) {
	int max = (imageSize.width > imageSize.height)? imageSize.width : imageSize.height;
	int ret = max * MEAN_BLOCK_SIZE_PER_SIZE_COEFFICIENT / distanceDivider;
	return (ret < 3)? 3 : ret + (ret + 1) % 2;
}

Mat QrBarcode::binarize(Image &image, int flags, int mean_C) {
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
			fillMissing(binarized, image);
		}
	}

	//DEBUG_WRITE_IMAGE("after_test_binarized.jpg", binarized);
	return binarized;
}

/*
 * QrBarcode.cpp
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../types.h"
#include "QrBarcode.h"

using namespace cv;
using namespace barcodes;

void QrBarcode::detect(Image &image, vector<DetectedMark> &detectedMarks, int flags) {
	if (image.data != NULL) {
		vector<DetectedMark> marks;
		int repairFlags = flags & (FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR);

		flags = flags & ~repairFlags;
		detectedMarks.clear();

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

void QrBarcode::detectByDistancePriority(Image &image, vector<DetectedMark> &detectedMarks, int flags) {
	vector<DetectedMark> marks;
	Mat binarized;
	int distanceFlags = flags & (FLAG_DISTANCE_FAR | FLAG_DISTANCE_MEDIUM | FLAG_DISTANCE_NEAR);
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
	}
}

void QrBarcode::filterMarks(vector<DetectedMark> &detectedMarks, double centerPointsMinDistance) {
	vector<DetectedMark>::iterator iter, iter2;

	for (iter = detectedMarks.begin(); iter != detectedMarks.end(); iter++) {
		RotatedRect refMarkBox = minAreaRect(Mat(iter->points));
		for (iter2 = detectedMarks.begin(); iter2 != detectedMarks.end(); ) {
			RotatedRect markBox = minAreaRect(Mat(iter2->points));
			Point centerDiff = Point(
				fabs(refMarkBox.center.x - markBox.center.x),
				fabs(refMarkBox.center.y - markBox.center.y)
			);
			if ((iter != iter2)
				&& (centerDiff.x / (double)refMarkBox.boundingRect().width < QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE)
				&& (centerDiff.y / (double)refMarkBox.boundingRect().height < QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE)
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

	  imshow( "aa", img_display );
	  imshow( "ff", result );
	  waitKey(0);

	  return minVal;
}


bool QrBarcode::intersection(Point point1, Point point2, Point _point1, Point _point2, Point &intersect) {
    Point x = _point1 - point1;
    Point d1 = point2 - point1;
    Point d2 = _point2 - _point1;

    float inter = d1.x*d2.y - d1.y*d2.x;

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
	}

	return dstImage;
}

void QrBarcode::_detect(Mat &image, vector<DetectedMark> &detectedMarks, int flags) {
	vector<vector<Point> > drawVec;
	Mat cropped, diff;
	vector<vector<Point> > contours;
	DetectedMark currMark;
	Mat contourImage = image.clone();
	Mat qrMark = buildQrMark();
	detectedMarks.clear();
	cout << "================ NEW CALL ================ " << endl;
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
			|| (boxSize.width < QR_MARK_MINIMAL_SIZE) ||(boxSize.height < QR_MARK_MINIMAL_SIZE)
			|| (image.cols /(double) boxSize.width < QR_MARK_MAXIMAL_SIZE_RATIO)
			|| (image.rows /(double) boxSize.height < QR_MARK_MAXIMAL_SIZE_RATIO))
			continue;

		/*Point2f hullMinBoxPoints[4]; box.points( hullMinBoxPoints);
		vector<Point> refShape;
		refShape.push_back(hullMinBoxPoints[0]);	refShape.push_back(hullMinBoxPoints[1]);
		refShape.push_back(hullMinBoxPoints[2]);	refShape.push_back(hullMinBoxPoints[3]);
		double match = matchShapes(Mat(hull), Mat(refShape), CV_CONTOURS_MATCH_I1, 0);
	    cout << "shape match:" << i << " : " << match << endl;
		if (match > QR_MARK_SHAPE_MATCH_ACCEPTED_RESULT) continue;*/
		double match;
		// Initialization histogram arduments
		MatND hist;
		int channels[] = {0};
		int histSize[] = {2};
		float range[] = {0, 256};
		const float* ranges[] = {range};

		// Getting the hull mask and cropping the contour image
		Mat contourMask = Mat::zeros(boxRect.size(), CV_8UC1);
		drawVec.push_back(contours[i]);
	    drawContours(contourMask, contours, i, Scalar(255), CV_FILLED, 8, noArray(), INT_MAX, Point(-boxRect.x, -boxRect.y));
		getRectSubPix(image, boxRect.size(), box.center, cropped);

		calcHist(&cropped, 1, channels, contourMask, hist, 1, histSize, ranges);
        float fill_density = hist.at<float>(0);
        float bg_density = hist.at<float>(1);

		if (bg_density < 1) continue;
        double bgFillRatio = fill_density / bg_density;
	    cout << "hist match:" << i << " : " << bgFillRatio << endl;
        if (fabs(bgFillRatio - QR_MARK_BG_FILL_RATIO) > QR_MARK_BG_FILL_RATIO_TOLERANCE) continue;



		// Converting the approx to the hull
		vector<Point> hull;
		convexHull(Mat(contours[i]), hull);
		Mat hullMask = Mat::zeros(boxRect.size(), CV_8UC1);
		drawVec.clear(); drawVec.push_back(hull);
	    drawContours(hullMask, drawVec, -1, Scalar(255), CV_FILLED, 8, noArray(), INT_MAX, Point(-boxRect.x, -boxRect.y));
	    int contourFill = cv::countNonZero(contourMask);
	    int hullFill = cv::countNonZero(hullMask);
	    cout << "cont/hull compare:" << i << " : " << contourFill << ":" << hullFill << ":" << boxRectSize.width * boxRectSize.height << endl;
	    //imshow("contourMask", contourMask); imshow("hullmask", hullMask);waitKey(0);
	    if (contourFill / (double) hullFill < QR_MARK_CONVEX_CONTOUR_MATCH) continue;

		int pointSize = (boxSize.width > boxSize.height)? boxSize.width / 7 : boxSize.height / 7;
		vector<Point> corners;
		findCorners(hull, 4, pointSize, corners);
	    cout << "corners found:" << corners.size() << endl;
		if (corners.size() != 4) continue;

		vector<Point> offsetCorners = corners;
		offsetContour(offsetCorners, Point(-boxRect.x, -boxRect.y));
		cropped = warpPerspective(cropped, offsetCorners);

		/*cvtColor(cropped, cropped, CV_GRAY2RGB);
		drawContours( cropped, drawVec, -1, CV_RGB(0,0,255), 4, 8, vector<Vec4i>(), 0, Point(- boxRect.x, - boxRect.y) );
		for( int j = 0; j < hull.size(); j++ )
			line( cropped, Point(hull[j].x - boxRect.x, hull[j].y - boxRect.y), Point(hull[j].x - boxRect.x, hull[j].y - boxRect.y), CV_RGB(255, 0, 0), 4, 8, 0);
		for( int j = 0; j < corners.size(); j++ )
			line( cropped, offsetCorners[j],offsetCorners[j], CV_RGB(255, 255, 0), 4, 8, 0);
		imwrite("out/" + toStr(i)  + ".jpg", cropped);continue;*/


        /*
        //imshow("before-mask", cropped);
        bitwise_not(contourMask, contourMask);
        cropped.setTo(Scalar(255), contourMask);

        //imshow("after-mask", cropped);
		Mat rotMat = getRotationMatrix2D(Point(boxCenter.x - boxRect.x, boxCenter.y - boxRect.y), box.angle, 1.0);
		Mat rotated;
		Size rotatedBoxSize = Size(50,50);
		/*if (box.angle < -45.)
		    swap(rotatedBoxSize.width, rotatedBoxSize.height);
		imshow("before-rotate", cropped);
		warpAffine(cropped, rotated, rotMat, rotatedBoxSize, INTER_CUBIC);
		imshow("after-rotate", rotated);waitKey(0);

		getRectSubPix(rotated, rotatedBoxSize, Point(rotated.cols / 2, rotated.rows / 2), rotated);
*/





		resize(cropped, cropped, Size(QR_MARK_TEMPLATE_SIZE, QR_MARK_TEMPLATE_SIZE));
        //match = matchTemplate(rotated, qrMark);

	    threshold(cropped, cropped, GLOBAL_THRESH, 255, CV_THRESH_OTSU);
	    cv::compare(cropped, qrMark, diff, cv::CMP_NE);
	    match = countNonZero(diff) / (double)(cropped.rows * cropped.cols);

	    currMark.flags = 0;
	    if (match > QR_MARK_TAMPLATE_MATCH_TOLERANCE) {
	    	if (!(flags & FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR)) {
	    		continue;
	    	}
	    	floodFill(cropped, Point(1, 1), Scalar(0));
	    	floodFill(cropped, Point(1, cropped.rows - 2), Scalar(0));
	    	floodFill(cropped, Point(cropped.cols - 2, 1), Scalar(0));
	    	floodFill(cropped, Point(cropped.cols - 2, cropped.rows - 2), Scalar(0));

	    	cv::compare(cropped, qrMark, diff, cv::CMP_NE);
	    	match = countNonZero(diff) / (double)(cropped.rows * cropped.cols);
	    	if (match / (double)(cropped.rows * cropped.cols) > QR_MARK_TAMPLATE_MATCH_TOLERANCE) continue;
	    	currMark.flags = FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR;
	    }
	    cout << "mark match: " << i << " : " << match << endl;

	    convexHull(corners, corners);
	    currMark.match = match;
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
	imwrite("before_test.jpg", image);

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

	imwrite("after_test_binarized.jpg", binarized);
	return binarized;
}

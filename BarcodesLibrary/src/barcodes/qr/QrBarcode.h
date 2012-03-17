/*
 * QrBarcode.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef QRBARCODE_H_
#define QRBARCODE_H_

#include "../Barcode.h"

namespace barcodes {
	using namespace std;
	using namespace cv;

	class QrBarcode: public Barcode {
	public:
		static const int FLAG_DISTANCE_NEAR                    = 0x01;
		static const int FLAG_DISTANCE_MEDIUM                  = 0x02;
		static const int FLAG_DISTANCE_FAR                     = 0x04;
		static const int FLAG_GLOBAL_THRESH                    = 0x08;
		static const int FLAG_ADAPT_THRESH                     = 0x10;
		static const int FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR = 0x20;
		static const int FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR  = 0x40;

		QrBarcode() {}
		virtual ~QrBarcode() {}

		virtual void detect(Image &image, vector<DetectedMark> &detectedMarks, int flags = FLAG_ADAPT_THRESH | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
				FLAG_DISTANCE_NEAR | FLAG_DISTANCE_MEDIUM | FLAG_DISTANCE_FAR);
	protected:
		struct PointNearbyPoints {
			Point point;
			vector<Point> nearbyLeftPoints;
			vector<Point> nearbyRightPoints;
		};

		static const int ROW_CUTS                                   =    10;
		static const int EDGE_DETECT_MEAN_BLOCK_SIZE                =    13;
		static const int MEAN_BLOCK_SIZE                            =    3;
		static const int EDGE_DETECT_MEAN_C                         =    43;
		static const int BINARIZE_MEAN_C                            =    7;
		static const int GLOBAL_THRESH                              =    125;
		static const double DISTANCE_NEAR_MEAN_BLOCK_SIZE_DIVIDER   =    1;
		static const double DISTANCE_MEDIUM_MEAN_BLOCK_SIZE_DIVIDER =    2;
		static const double DISTANCE_FAR_MEAN_BLOCK_SIZE_DIVIDER    =    6;
		static const double MEAN_BLOCK_SIZE_PER_SIZE_COEFFICIENT    =    0.2166;
		static const double QR_MARK_SHAPE_MATCH_ACCEPTED_RESULT     =    0.025;
		static const int QR_MARK_MINIMAL_SIZE                       =    14;
		static const double QR_MARK_MAXIMAL_SIZE_RATIO              =    2.0;
		static const double QR_MARK_BOUNDING_RECT_MAX_ACCEPTED_SCALE=    4.0;
		static const double QR_MARK_BG_FILL_RATIO                   =    2.0625;
		static const double QR_MARK_BG_FILL_RATIO_TOLERANCE         =    1.5;
		static const double QR_MARK_TEMPLATE_SIZE                   =    280;
		static const double QR_MARK_CONVEX_CONTOUR_MATCH            =    0.9;
		static const double QR_MARK_MINIMAL_CORNER_ANGLE            =    35; // stupnu
		static const double QR_MARK_OPTIMAL_CORNER_ANGLE            =    90; // stupnu
		static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE        =    0.2;
		static const double QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE  =    0.2;

		void _detect(Mat &image, vector<DetectedMark> &detectedMarks, int flags = 0);
		void detectByDistancePriority(Image &image, vector<DetectedMark> &detectedMarks, int flags);
		void filterMarks(vector<DetectedMark> &detectedMarks, double centerPointsMinDistance = QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE);
		Mat buildQrMark();
		double matchTemplate(Mat img, Mat imgTemplate);
		void rotateContour(vector<Point> &contour, Point2f pivot, float angle);
		void offsetContour(vector<Point> &contour, Point2f offset);
		int getBlockSize(Size imageSize, double distanceDivider);
		void findCorners(vector<Point> &shapePoints, int cornerCount, int minCornerArea, vector<Point> &cornerPoints, double minCornerAngle = QR_MARK_MINIMAL_CORNER_ANGLE, double optimalAngle = QR_MARK_OPTIMAL_CORNER_ANGLE);
		void getNearbyPoints(vector<Point> &shapePoints, vector<PointNearbyPoints> &pointNearbyPoints, vector<Point> &cornerArea);
		void filterPointsInArea(vector<Point> &points, vector<Point> &area);
		Mat warpPerspective(Mat &image, vector<Point> &corners);
		bool intersection(Point point1, Point point2, Point _point1, Point _point2, Point &intersect);
		Mat binarize(Image &image, int flags, int mean_C = BINARIZE_MEAN_C);
		uchar rowMeanAvg(Mat &image, int row, int from, int to, int rowCuts = ROW_CUTS);
		void fillMissing(Mat &means, Mat &refImage, int image_EdgeDetect_MeanBlockSize = EDGE_DETECT_MEAN_BLOCK_SIZE,
		int refImage_MeanBlockSize = MEAN_BLOCK_SIZE, int image_EdgeDetect_Mean_C = EDGE_DETECT_MEAN_C);
	};

} /* namespace barcodes */
#endif /* QRBARCODE_H_ */

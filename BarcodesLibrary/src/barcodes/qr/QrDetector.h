/*
 * QrDetector.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef QRDETECTOR_H_
#define QRDETECTOR_H_

#include "../Detector.h"

namespace barcodes {
using namespace std;
using namespace cv;

class QrDetector: public Detector {
public:
	static const int FLAG_DISTANCE_NEAR                    = 0x0001;
	static const int FLAG_DISTANCE_MEDIUM                  = 0x0002;
	static const int FLAG_DISTANCE_FAR                     = 0x0004;
	static const int FLAG_DISTANCE_FAR_FAR                 = 0x0008;
	static const int FLAG_GLOBAL_THRESH                    = 0x0010;
	static const int FLAG_ADAPT_THRESH                     = 0x0020;
	static const int FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR = 0x0040;
	static const int FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR  = 0x0080;
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_LOW      = 0x0100;
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL   = 0x0200;
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_HIGH     = 0x0400;

	static const int REPAIR_FLAGS = FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR;
	static const int DISTANCE_FLAGS = FLAG_DISTANCE_FAR_FAR | FLAG_DISTANCE_FAR | FLAG_DISTANCE_MEDIUM | FLAG_DISTANCE_NEAR;
	static const int MATCH_TOLERANCE_FLAGS = FLAG_QR_MARK_MATCH_TOLERANCE_LOW | FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL | FLAG_QR_MARK_MATCH_TOLERANCE_HIGH;

	void detect(Image &image, DetectedMarks &detectedMarks, int flags = FLAG_ADAPT_THRESH | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
			DISTANCE_FLAGS | FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL) const;
	const Detector *getDecoder() const;

	static const QrDetector *getInstance();

	static Mat binarize(Image &image, int flags, int mean_C = BINARIZE_MEAN_C);
protected:
	static const QrDetector DETECTOR_INSTANCE;

	static const int ROW_CUTS                                   =    7;
	static const int EDGE_DETECT_MEAN_BLOCK_SIZE                =    13;
	static const int MEAN_BLOCK_SIZE                            =    3;
	static const int EDGE_DETECT_MEAN_C                         =    43;
	static const int BINARIZE_MEAN_C                            =    7;
	static const int GLOBAL_THRESH                              =    125;
	static const double MEAN_BLOCK_SIZE_PER_SIZE_COEFFICIENT    =    0.2166;
	static const double QR_MARK_SHAPE_MATCH_ACCEPTED_RESULT     =    0.025;
	static const double QR_MARK_MAXIMAL_SIZE_RATIO              =    2.0;
	static const double QR_MARK_BOUNDING_RECT_MAX_ACCEPTED_SCALE=    4;
	static const double QR_MARK_TEMPLATE_SIZE                   =    35;
	static const double QR_MARK_CONVEX_CONTOUR_MATCH            =    0.9;
	static const double QR_MARK_MINIMAL_CORNER_ANGLE            =    35; // stupnu
	static const double QR_MARK_OPTIMAL_CORNER_ANGLE            =    90; // stupnu
	static const double QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE  =    0.2;

	static const double QR_MARK_BG_FILL_RATIO_MAX               =    6.125;    // -8 white points (reference 49 / 16)
	static const double QR_MARK_BG_FILL_RATIO_MIN               =    2.041666; // +8 white points (reference 49 / 16)

	static const double DISTANCE_NEAR_MEAN_BLOCK_SIZE_DIVIDER   =    1;
	static const double DISTANCE_MEDIUM_MEAN_BLOCK_SIZE_DIVIDER =    3;
	static const double DISTANCE_FAR_MEAN_BLOCK_SIZE_DIVIDER    =    6;
	static const double DISTANCE_FAR_FAR_MEAN_BLOCK_SIZE_DIVIDER=    12;

	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_HIGH   =    0.4;
	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_NORMAL =    0.2;
	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_LOW    =    0.1;

	static const int QR_MARK_MINIMAL_SIZE_SMALL                 =    10;
	static const int QR_MARK_MINIMAL_SIZE_NORMAL                =    14;
	static const int QR_MARK_MINIMAL_SIZE_LARGE                 =    20;

	QrDetector() {}
	virtual ~QrDetector() {}

	void _detect(Mat &image,DetectedMarks &detectedMarks, int flags = 0) const;
	void detectByDistancePriority(Image &image,DetectedMarks &detectedMarks, int flags) const;
	static int getBlockSize(Size imageSize, double distanceDivider);
};

} /* namespace barcodes */
#endif /* QRDETECTOR_H_ */

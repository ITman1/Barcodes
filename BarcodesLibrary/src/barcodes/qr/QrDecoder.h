/*
 * QrDecoder.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef QRDECODER_H_
#define QRDECODER_H_

#include <opencv2/core/core.hpp>

#include "../../common.h"
#include "../Decoder.h"
#include "QrDetector.h"

namespace barcodes {
using namespace cv;

class QrDecoder: public Decoder {
public:
	QrDecoder() {}
	virtual ~QrDecoder() {}

	virtual void decode(Image &image, ByteArray &data, int flags = QrDetector::FLAG_ADAPT_THRESH |
			QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
			QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);
protected:
	static const QrDetector QR_BARCODE_DETECTOR;

	static const int QR_CODE_WARP_PERSPECTIVE_SIZE              =    1000;

	static const double SAMPLE_RECT_START_ANGLE_SHIFT           =    10;
	static const double SAMPLE_RECT_ANGLE_STEP                  =    0.5;
	static const double SAMPLE_RECT_MAX_ANGLE                   =    20;
	static const double SAMPLE_RECT_EDGE_DETECT_FILL_RATIO      =    0.05;
	static const double SAMPLE_RECT_FINAL_STEPS_COUNT           =    10;

	void _read_V2_40(Image &image, ByteArray &data, DetectedMarks &detectedMarks, int flags = 0);
	void getPerspectiveCorners_V1_40(Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners);
	bool sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample);
	double _sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth);
};

} /* namespace barcodes */
#endif /* QRDECODER_H_ */

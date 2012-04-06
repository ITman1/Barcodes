/*
 * QrDecoder.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef QRDECODER_H_
#define QRDECODER_H_

#include <opencv2/core/core.hpp>

#include "../Decoder.h"
#include "../../common/Vector2D.h"
#include "QrDetector.h"

namespace barcodes {
using namespace cv;

class QrDecoder: public Decoder {
public:
	void decode(Image &image, DataSegments &dataSegments, int flags = QrDetector::FLAG_ADAPT_THRESH |
			QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
			QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL) const;
	const Decoder *getDecoder() const;

	static const QrDecoder *getInstance();
protected:
	static const QrDecoder DECODER_INSTANCE;

	static const int QR_CODE_WARP_PERSPECTIVE_SIZE              =    1000;

	static const double SAMPLE_RECT_START_ANGLE_SHIFT           =    10;
	static const double SAMPLE_RECT_ANGLE_STEP                  =    0.5;
	static const double SAMPLE_RECT_MAX_ANGLE                   =    20;
	static const double SAMPLE_RECT_EDGE_DETECT_FILL_RATIO      =    0.05;
	static const double SAMPLE_RECT_FINAL_STEPS_COUNT           =    10;

	static const Size CODEWORD_SAMPLE_SIZE;

	QrDecoder() {}
	virtual ~QrDecoder() {}

	void _read_V1_40(Image &image, DataSegments &dataSegments, DetectedMarks &detectedMarks, int flags = 0) const;
	void getPerspectiveCorners_V1_40(Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const;
	bool sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample) const;
	double _sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) const;
};

} /* namespace barcodes */
#endif /* QRDECODER_H_ */

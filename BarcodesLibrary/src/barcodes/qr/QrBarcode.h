/*
 * QrBarcode.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef QRBARCODE_H_
#define QRBARCODE_H_

#include "../Barcode.h"
#include "QrDetector.h"
#include "QrDecoder.h"

namespace barcodes {
	using namespace std;
	using namespace cv;

	class QrBarcode: public Barcode {
	public:
		QrBarcode() {}
		virtual ~QrBarcode() {}

		static void detect(Image &image, DetectedMarks &detectedMarks, int flags = QrDetector::FLAG_ADAPT_THRESH |
				QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
				QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);
		static void decode(Image &image, vector<DataSegment> &dataSegments, int flags = QrDetector::FLAG_ADAPT_THRESH |
				QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
				QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);
	};

} /* namespace barcodes */
#endif /* QRBARCODE_H_ */

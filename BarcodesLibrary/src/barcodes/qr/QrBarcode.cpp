/*
 * QrBarcode.cpp
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */


#include "QrBarcode.h"

#define DEBUG_TAG "QrBarcode.cpp"

namespace barcodes {
void QrBarcode::detect(Image &image, DetectedMarks &detectedMarks, int flags) {
	QrDetector::getInstance()->detect(image, detectedMarks, flags);
}

void QrBarcode::decode(Image &image, vector<DataSegment> &dataSegments, int flags) {
	QrDecoder::getInstance()->decode(image, dataSegments, flags);
}

}




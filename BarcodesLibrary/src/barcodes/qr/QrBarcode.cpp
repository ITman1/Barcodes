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
	QrDetector().detect(image, detectedMarks, flags);
}

void QrBarcode::decode(Image &image, ByteArray &data, int flags) {
	QrDecoder().decode(image, data, flags);
}

}




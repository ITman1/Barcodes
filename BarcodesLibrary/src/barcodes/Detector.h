/*
 * Detector.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include "../common/Image.h"
#include "DetectedMarks.h"

namespace barcodes {
using namespace std;

class Detector {
public:
	Detector() {}
	virtual ~Detector() {}

	virtual void detect(Image &image, DetectedMarks &detectedMarks, int flags);
};

} /* namespace barcodes */
#endif /* DETECTOR_H_ */

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
protected:
	Detector() {}
	virtual ~Detector() {}

public:
	virtual void detect(Image &image, DetectedMarks &detectedMarks, int flags) const = 0;
	virtual const Detector *getDecoder() const = 0;
};

} /* namespace barcodes */
#endif /* DETECTOR_H_ */

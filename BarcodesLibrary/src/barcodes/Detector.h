///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Detector.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines Detector class that only specifies the interface for
//             creation of all detector classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Detector.h
 *
 * @brief Defines Detector class that only specifies the interface for
 *        creation of all detector classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include "../common/Image.h"
#include "DetectedMarks.h"

namespace barcodes {
using namespace std;

/**
 * Abstract base class for creation of the classes which serves for detection
 * of barcodes in the image.
 */
class Detector {
protected:
	Detector() {}
	virtual ~Detector() {}

public:

	/**
	 * Detects barcode and returns localization marks/points.
	 *
	 * @param image Image with the barcode.
	 * @param detectedMarks Recognized localization marks/points.
	 * @param flags Flags which might be used for detection process.
	 */
	virtual void detect(Image &image, DetectedMarks &detectedMarks, int flags) const = 0;

};

} /* namespace barcodes */
#endif /* DETECTOR_H_ */

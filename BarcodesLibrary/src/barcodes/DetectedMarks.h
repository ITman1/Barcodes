/*
 * DetectedMark.h
 *
 *  Created on: 24.3.2012
 *      Author: Scotty
 */

#ifndef DETECTEDMARK_H_
#define DETECTEDMARK_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;
class DetectedMark {
public:
	vector<Point> points;
	double match;
	int flags;

	DetectedMark() {}
	virtual ~DetectedMark() {}
};

class DetectedMarks: public vector<DetectedMark> {
public:
	DetectedMarks() {}
	virtual ~DetectedMarks() {}

	void filter(double centerPointsMinDistance);
	void perspectiveTransform(Mat &transformation);

};
}



#endif /* DETECTEDMARK_H_ */

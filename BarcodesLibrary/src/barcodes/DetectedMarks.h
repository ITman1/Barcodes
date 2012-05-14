///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       DetectedMarks.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines DetectedMark and DetectedMarks classes which are returned
//             by detect methods and contains detected localization marks.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file DetectedMarks.h
 *
 * @brief Defines DetectedMark and DetectedMarks classes which are returned
 *        by detect methods and contains detected localization marks.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef DETECTEDMARK_H_
#define DETECTEDMARK_H_

#include <opencv2/core/core.hpp>

namespace barcodes {
using namespace cv;

/**
 * Class which holds detected points of one detected mark.
 */
class DetectedMark {
public:
	vector<Point> points; /**< Points of the mark */
	double match;		  /**< Match ratio and possibility that this is the mark */
	int flags;			  /**< Flags/Conditions in which has been detected this mark */
	int variant;/**< Variant object attribute, it can serve for passing int value or pointer etc. */

	DetectedMark() : match(0), flags(0), variant(0) {}

	virtual ~DetectedMark() {}
};

/**
 * Class which holds all detected marks and adds some other methods.
 *
 * @see DetectedMark
 */
class DetectedMarks: public vector<DetectedMark> {
public:
	virtual ~DetectedMarks() {}

	/**
	 * Filters detected marks which too close.
	 *
	 * @param centerPointsMinDistance Minimal possible allowed distance ratio between centers of the marks.
	 *        Minimal distance is calculated as a product of multiplying sizes of bounding rectangle and this ratio.
	 */
	void filter(double centerPointsMinDistance);

	/**
	 * Perspective transform of the marks.
	 *
	 * @param transformation Transformation matrix.
	 */
	void perspectiveTransform(Mat &transformation);

};
}



#endif /* DETECTEDMARK_H_ */

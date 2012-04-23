///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       PerspCornersFromLineSampling.h
// Date:       April 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines functor class for retrieving the perspective corners
//             of the QR code in the image using line sampling for that.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file PerspCornersFromLineSampling.h
 *
 * @brief Defines functor class for retrieving the perspective corners
 * of the QR code in the image using line sampling for that.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef PERSPCORNERSFROMLINESAMPLING_H_
#define PERSPCORNERSFROMLINESAMPLING_H_

#include "GetPerspCorners.h"

namespace barcodes {
using namespace cv;

/**
 * Functor class which serves for retrieving the perspective corners of
 * the QR code in the image. For getting the fourth corner it especially uses
 * sampling method.
 */
class PerspCornersFromLineSampling: public GetPerspCorners {
protected:
	/**
	 * The initial inner offset of the sample rectangle which samples 4. corner of the QR code.
	 */
	static const double SAMPLE_RECT_START_ANGLE_SHIFT           =    10;

	/**
	 * One step of the sample rectangle which samples 4. corner of the QR code.
	 */
	static const double SAMPLE_RECT_ANGLE_STEP                  =    0.5;

	/**
	 * The maximal angle about which rotated the sample rectangle which samples the 4. corner of the QR code.
	 */
	static const double SAMPLE_RECT_MAX_ANGLE                   =    20;

	/**
	 * Sampling of the 4. corner ends when sampling rectangle samples this ratio or lower.
	 */
	static const double SAMPLE_RECT_EDGE_DETECT_FILL_RATIO      =    0.05;

	/**
	 * The number of final steps for more precise result.
	 */
	static const double SAMPLE_RECT_FINAL_STEPS_COUNT           =    10;

	/**
	 * Temporary matrix for calculations.
	 */
	mutable Mat transformed;

	/**
	 * Proceeds sampling by rectangle.
	 *
	 * @param binarized Binarized image.
	 * @param sampleVector The vector which defines the direction and sizes of the sample rectangle.
	 * @param rotatePoint Point around which should be rotated the sample rectangle.
	 * @param lineShift Offset of the sample rectangle.
	 * @param lineWidth Width of the sample rectangle.
	 * @param clockWiseSample Direction of the sampling.
	 * @return Returns true if maximal sample rotation angle is not exceeded, otherwise false.
	 */
	bool sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth, bool clockWiseSample) const;

	/**
	 * Proceeds one sample and returns background fill ratio.
	 *
	 * @param binarized Binarized image.
	 * @param sampleVector The vector which defines the direction and sizes of the sample rectangle.
	 * @param rotatePoint Point around which should be rotated the sample rectangle.
	 * @param lineShift Offset of the sample rectangle.
	 * @param lineWidth Width of the sample rectangle.
	 * @return Fill ratio of the background.
	 */
	double _sampleQrCodeEdge(Mat &binarized, Vector2Df &sampleVector, Point2f &rotatePoint, Vector2Df &lineShift, int lineWidth) const;

public:
	/**
	 * Retrieves four corners for perspective transformation and sorts the
	 * detected marks and theirs points as defined bellow.
	 *
	 * \code
	 *  Getting the A,B,C,D points, sorted as C,D,A,B for perspective transformation
	 *
	 *  A,B,C are retrieved from the X,Y,Z
	 *
	 *  X,Y,Z are the points with the minimal inner/outer distance from the polygon (triangle)
	 *  which is made by the center points of QR marks.
	 *
	 *  D point is sampled by two lines which has rotate pivot in the N and M.
	 *  These lines start sampling inside QR code and goes outside and when there is
	 *  a white background they ends. D is calculated as an intersect of these lines.
	 *
	 *  B******           ******C
	 *  * *** *           * *** *
	 *  * *** *-----------* *** *
	 *  * *** *           * *** *
	 *  ******Y           Z*****N
	 *     |            /
	 *     |         /
	 *     |       /
	 *     |    /
	 *  ******X
	 *  * *** *
	 *  * *** *
	 *  * *** *
	 *  A*****M                 D
	 * \endcode
	 *
	 * @param binarized Binarized image with the QR code.
	 * @param detectedMarks Detected marks. These marks are sorted.
	 * @param corners Result corners for perspective transformation.
	 */
	void getPerspectiveCorners(Mat &image, Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const;
};

} /* namespace barcodes */
#endif /* PERSPCORNERSFROMLINESAMPLING_H_ */

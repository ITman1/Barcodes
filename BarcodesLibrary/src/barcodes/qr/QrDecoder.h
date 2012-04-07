///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDecoder.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDecoder class that implements decoder of the QR
//             codes in the image.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDecoder.h
 *
 * @brief Defines QrDecoder class that implements decoder of the QR
 *        codes in the image.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDECODER_H_
#define QRDECODER_H_

#include <opencv2/core/core.hpp>

#include "../Decoder.h"
#include "../../common/Vector2D.h"
#include "QrDetector.h"

namespace barcodes {
using namespace cv;

/**
 * Class of the singleton used for decoding of the QR code in the image.
 */
class QrDecoder: public Decoder {
public:

	/**
	 * Decodes QR code on the image and returns decoded data segments.
	 *
	 * @param image Image with the QR code.
	 * @param dataSegments Result decoded data segments.
	 * @param flags Flags used for detection and decoding.
	 */
	void decode(Image &image, DataSegments &dataSegments, int flags = QrDetector::FLAG_ADAPT_THRESH |
			QrDetector::REPAIR_FLAGS | QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL) const;

	/**
	 * Returns instance of the QR decoder.
	 *
	 * @return Instance of the QR decoder.
	 */
	static const QrDecoder *getInstance();
protected:
	/**
	 * Instance of the QR decoder.
	 */
	static const QrDecoder DECODER_INSTANCE;

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
	 * The size of the sampling grid for retrieving the data from the bit matrix.
	 */
	static const Size CODEWORD_SAMPLE_SIZE;

	QrDecoder() {}
	virtual ~QrDecoder() {}

	/**
	 * Decodes QR code of the versions 1-40 and returns decoded data segments.
	 *
	 * @param image Image with the QR code.
	 * @param dataSegments Result decoded data segments.
	 * @param detectedMarks The localization marks.
	 * @param flags Flags used for detection and decoding.
	 */
	void _read_V1_40(Image &image, DataSegments &dataSegments, DetectedMarks &detectedMarks, int flags = 0) const;

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
	void getPerspectiveCorners_V1_40(Mat &binarized, DetectedMarks &detectedMarks, vector<Point> &corners) const;

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
};

} /* namespace barcodes */
#endif /* QRDECODER_H_ */

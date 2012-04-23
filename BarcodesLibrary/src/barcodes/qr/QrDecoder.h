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
#include "QrDetector.h"
#include "perspcorners/GetPerspCorners.h"

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
	 * Returns last processed image during reading.
	 *
	 * @return Last processed image during reading.
	 */
	Image lastProcessedImage() const;

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
	 * The size of the sampling grid for retrieving the data from the bit matrix.
	 */
	static const Size CODEWORD_SAMPLE_SIZE;

	/**
	 * Last processed wrapped image.
	 */
	mutable Mat warpedImage;

	QrDecoder() {}
	virtual ~QrDecoder() {}

	/**
	 * Decodes QR code of the versions 1-40 and returns decoded data segments.
	 *
	 * @param image Image with the QR code.
	 * @param binarized Binarized image by the values from the detection.
	 * @param dataSegments Result decoded data segments.
	 * @param detectedMarks The localization marks.
	 * @param fun Functor for getting the perspective corners.
	 * @param flags Flags used for detection and decoding.
	 */
	void _read_V1_40(Image &image, Mat &binarized, DataSegments &dataSegments, DetectedMarks &detectedMarks, GetPerspCorners &perspCornersFunct) const;

	/**
	 * Decodes QR code of the versions 1-40 and returns decoded data segments.
	 *
	 * @param image Image with the QR code.
	 * @param dataSegments Result decoded data segments.
	 * @param detectedMarks The localization marks.
	 * @param flags Flags used for detection and decoding.
	 */
	void read_V1_40(Image &image, DataSegments &dataSegments, DetectedMarks &detectedMarks, int flags = 0) const;
};

} /* namespace barcodes */
#endif /* QRDECODER_H_ */

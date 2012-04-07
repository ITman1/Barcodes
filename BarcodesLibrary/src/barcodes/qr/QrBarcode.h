///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrBarcode.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrBarcode base class which should be used for
//             the creation of specific QR code classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrBarcode.h
 *
 * @brief Defines QrBarcode base class which should be used for
 *        the creation of specific QR code classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRBARCODE_H_
#define QRBARCODE_H_

#include "../Barcode.h"
#include "QrDetector.h"
#include "QrDecoder.h"

namespace barcodes {
	using namespace std;
	using namespace cv;

	/**
	 * Base class for creation of the QR code classes.
	 */
	class QrBarcode: public Barcode {
	public:
		QrBarcode() {}
		virtual ~QrBarcode() {}

		/**
		 * Detects QR code and returns localization marks.
		 *
		 * @param image Image with the QR code.
		 * @param detectedMarks Recognized localization marks.
		 * @param flags Detection flags.
		 */
		static void detect(Image &image, DetectedMarks &detectedMarks, int flags = QrDetector::FLAG_ADAPT_THRESH |
				QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
				QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);

		/**
		 * Decodes QR code on the image and returns decoded data segments.
		 *
		 * @param image Image with the QR code.
		 * @param dataSegments Result decoded data segments.
		 * @param flags Flags used for detection and decoding.
		 */
		static void decode(Image &image, DataSegments &dataSegments, int flags = QrDetector::FLAG_ADAPT_THRESH |
				QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR | QrDetector::FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR |
				QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);
	};

} /* namespace barcodes */
#endif /* QRBARCODE_H_ */

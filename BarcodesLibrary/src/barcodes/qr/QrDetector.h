///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDetector.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrDetector class that implements detection of the QR
//             codes in the image.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDetector.h
 *
 * @brief Defines QrDetector class that implements detection of the QR
 *        codes in the image.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRDETECTOR_H_
#define QRDETECTOR_H_

#include "../Detector.h"

namespace barcodes {
using namespace std;
using namespace cv;

/**
 * Class of the singleton used for detection of QR code in the image.
 */
class QrDetector: public Detector {
public:
	/**
	 * Detection flag. Detect QR code that occupies almost whole image.
	 *
	 * @see DISTANCE_NEAR_MEAN_BLOCK_SIZE_DIVIDER
	 * @see getBlockSize
	 */
	static const int FLAG_DISTANCE_NEAR                    = 0x0001;

	/**
	 * Detection flag. Detect QR code that occupies medium part of the image.
	 *
	 * @see DISTANCE_MEDIUM_MEAN_BLOCK_SIZE_DIVIDER
	 * @see getBlockSize
	 */
	static const int FLAG_DISTANCE_MEDIUM                  = 0x0002;

	/**
	 * Detection flag. Detect QR code that occupies a small part of the image.
	 *
	 * @see DISTANCE_FAR_MEAN_BLOCK_SIZE_DIVIDER
	 * @see getBlockSize
	 */
	static const int FLAG_DISTANCE_FAR                     = 0x0004;

	/**
	 * Detection flag. Detect QR code that occupies very small part of the image.
	 *
	 * @see DISTANCE_FAR_FAR_MEAN_BLOCK_SIZE_DIVIDER
	 * @see getBlockSize
	 */
	static const int FLAG_DISTANCE_FAR_FAR                 = 0x0008;

	/**
	 * Detection flag. Binarize input image with global threshold.
	 *
	 * @see GLOBAL_THRESH
	 */
	static const int FLAG_GLOBAL_THRESH                    = 0x0010;

	/**
	 * Detection flag. Binarize input image with adaptive/local threshold.
	 *
	 * @see BINARIZE_MEAN_C
	 * @see getBlockSize
	 */
	static const int FLAG_ADAPT_THRESH                     = 0x0020;

	/**
	 * Detection flag. Applies repair of the binarization if needed.
	 * This repair fills the blank places which might be a product of badly
	 * chosen C mean constant for adaptive threshold.
	 *
	 * @see getBlockSize
	 * @see fillMissing
	 */
	static const int FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR = 0x0040;

	/**
	 * Detection flag. Applies outer fill for detected finder pattern
	 * before match (if needed).
	 */
	static const int FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR  = 0x0080;

	/**
	 * Detection flag. Which sets the exact match of the finder pattern
	 * to low values.
	 */
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_LOW      = 0x0100;

	/**
	 * Detection flag. Which sets the exact match of the finder pattern
	 * to normal values.
	 */
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL   = 0x0200;

	/**
	 * Detection flag. Which sets the exact match of the finder pattern
	 * to high values.
	 */
	static const int FLAG_QR_MARK_MATCH_TOLERANCE_HIGH     = 0x0400;

	/**
	 * Hierarchy flag. Defines whether to use the hierarchy for the detection.
	 */
	static const int FLAG_USE_HIERARCHY                    = 0x0800;

	/**
	 * Groups all repair flags.
	 */
	static const int REPAIR_FLAGS = FLAG_ADAPT_THRESH_CORRUPT_FILL_REPAIR | FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR;

	/**
	 * Groups all distance flags.
	 */
	static const int DISTANCE_FLAGS = FLAG_DISTANCE_FAR_FAR | FLAG_DISTANCE_FAR | FLAG_DISTANCE_MEDIUM | FLAG_DISTANCE_NEAR;

	/**
	 * Groups all match tolerance flags.
	 */
	static const int MATCH_TOLERANCE_FLAGS = FLAG_QR_MARK_MATCH_TOLERANCE_LOW | FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL | FLAG_QR_MARK_MATCH_TOLERANCE_HIGH;

	/**
	 * Detects QR code and returns localization marks.
	 *
	 * @param image Image with the QR code.
	 * @param detectedMarks Recognized localization marks.
	 * @param flags Detection flags.
	 */
	void detect(Image &image, DetectedMarks &detectedMarks, int flags = FLAG_ADAPT_THRESH | REPAIR_FLAGS |
			DISTANCE_FLAGS | FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL) const;

	/**
	 * Returns instance of the QR detector.
	 *
	 * @return Instance of the QR detector.
	 */
	static const QrDetector *getInstance();

	/**
	 * Binarize image to 0 and 255 values.
	 *
	 * @param image Input image.
	 * @param flags Flags for specifying the distance of the QR code and type of threshold.
	 * @param mean_C Constant for adaptive threshold which offsets threshold value.
	 * @return Binarized image.
	 */
	static Mat binarize(Mat &image, int flags, int mean_C = BINARIZE_MEAN_C);
protected:
	/**
	 * Offset for storing contours indices. Is changed every time after detect call.
	 */
	mutable int contourOffset;

	/**
	 * Instance of the QR detector.
	 */
	static const QrDetector DETECTOR_INSTANCE;

	/**
	 * The number of samples which will be used for counting average mean on the row
	 * in the fillMissing function.
	 */
	static const int ROW_CUTS                                   =    7;

	/**
	 * Block size used for calculation of means and detection of the edges
	 * inside the fillMissing function.
	 */
	static const int EDGE_DETECT_MEAN_BLOCK_SIZE                =    13;

	/**
	 * Block size used for calculation of means for reference image
	 * inside the fillMissing function.
	 */
	static const int MEAN_BLOCK_SIZE                            =    3;

	/**
	 * The tolerance constant for edge detection inside the fillMissing function.
	 */
	static const int EDGE_DETECT_MEAN_C                         =    43;

	/**
	 * The constant for adaptive threshold about which is offset ideal threshold value
	 *
	 * pixel = ( pixel > mean - C ) ? fill : bg
	 */
	static const int BINARIZE_MEAN_C                            =    7;

	/**
	 * The value of the global threshold used for binarization.
	 */
	static const int GLOBAL_THRESH                              =    125;

	/**
	 * The coefficient used for calculation of the block size for adaptive threshold.
	 *
	 * @see getBlockSize
	 */
	static const double MEAN_BLOCK_SIZE_PER_SIZE_COEFFICIENT    =    0.2166;

	/**
	 * Detection filter. Finder pattern cannot be larger then size of the image which
	 * is divided by this constant.
	 */
	static const double QR_MARK_MAXIMAL_SIZE_RATIO              =    2.0;

	/**
	 * (Detection filter) Filtering by size of the bounding rectangle.
	 * How many times can differ one side of the rectangle from the other.
	 */
	static const double QR_MARK_BOUNDING_RECT_MAX_ACCEPTED_SCALE=    4;

	/**
	 * Size of the template for exact match of finder pattern.
	 */
	static const double QR_MARK_TEMPLATE_SIZE                   =    35;

	/**
	 * (Detection filter) Match which has to pass hull and contours polygons.
	 */
	static const double QR_MARK_CONVEX_CONTOUR_MATCH            =    0.9;

	/**
	 * The minimal angle accepted for a corner.
	 */
	static const double QR_MARK_MINIMAL_CORNER_ANGLE            =    35; // degrees

	/**
	 * The optimal angle accepted for a corner.
	 */
	static const double QR_MARK_OPTIMAL_CORNER_ANGLE            =    90; // stupnu

	/**
	 * The distance ratio for filtering the detected finder marks
	 * and supposing that they are the same.
	 *
	 * @see DetectedMarks::filter
	 */
	static const double QR_MARK_CENTER_POINTS_MINIMUM_DISTANCE  =    0.2;

	/**
	 * (Detection filter) The maximal accepted background/fill ratio for histogram match.
	 */
	static const double QR_MARK_BG_FILL_RATIO_MAX               =    6.125;    // -8 white points (reference 49 / 16 = 3.0625)

	/**
	 * (Detection filter) The minimal accepted background/fill ratio for histogram match.
	 */
	static const double QR_MARK_BG_FILL_RATIO_MIN               =    2.041666; // +8 white points (reference 49 / 16 = 3.0625)

	/**
	 * Divider of calculated block size for near QR codes.
	 *
	 * @see FLAG_DISTANCE_NEAR
	 */
	static const double DISTANCE_NEAR_MEAN_BLOCK_SIZE_DIVIDER   =    1;

	/**
	 * Divider of calculated block size for QR codes in medium distance.
	 *
	 * @see FLAG_DISTANCE_MEDIUM
	 */
	static const double DISTANCE_MEDIUM_MEAN_BLOCK_SIZE_DIVIDER =    3;

	/**
	 * Divider of calculated block size for far QR codes.
	 *
	 * @see FLAG_DISTANCE_FAR
	 */
	static const double DISTANCE_FAR_MEAN_BLOCK_SIZE_DIVIDER    =    6;

	/**
	 * Divider of calculated block size for very far QR codes.
	 *
	 * @see FLAG_DISTANCE_FAR_FAR
	 */
	static const double DISTANCE_FAR_FAR_MEAN_BLOCK_SIZE_DIVIDER=    12;

	/**
	 * The match tolerance by which can differ the finder pattern from the template. (High tolerance)
	 *
	 * @see FLAG_QR_MARK_MATCH_TOLERANCE_HIGH
	 */
	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_HIGH   =    0.4;

	/**
	 * The match tolerance by which can differ the finder pattern from the template. (Normal tolerance)
	 *
	 * @see FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL
	 */
	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_NORMAL =    0.2;

	/**
	 * The match tolerance by which can differ the finder pattern from the template. (Low tolerance)
	 *
	 * @see FLAG_QR_MARK_MATCH_TOLERANCE_LOW
	 */
	static const double QR_MARK_TAMPLATE_MATCH_TOLERANCE_LOW    =    0.1;

	/**
	 * The minimal size which can have finder pattern. (Small)
	 */
	static const int QR_MARK_MINIMAL_SIZE_SMALL                 =    10;

	/**
	 * The minimal size which can have finder pattern. (Medium)
	 */
	static const int QR_MARK_MINIMAL_SIZE_NORMAL                =    14;

	/**
	 * The minimal size which can have finder pattern. (Large)
	 */
	static const int QR_MARK_MINIMAL_SIZE_LARGE                 =    20;

	QrDetector() : contourOffset(0) {}
	virtual ~QrDetector() {}

	/**
	 * Detects QR code and returns localization marks.
	 *
	 * @param image Image with the QR code.
	 * @param detectedMarks Recognized localization marks.
	 * @param flags Match tolerance/Repair flags.
	 */
	void _detect(Mat &image,DetectedMarks &detectedMarks, int flags = 0) const;

	/**
	 * Detects QR code and returns localization marks.
	 *
	 * @param image Image with the QR code.
	 * @param detectedMarks Recognized localization marks.
	 * @param flags Match tolerance/Distance/Repair flags.
	 */
	void detectByDistancePriority(Image &image,DetectedMarks &detectedMarks, int flags) const;

	/**
	 * Calculates size of the block for adaptive threshold for specified distance divider.
	 *
	 * @param imageSize Size of the image.
	 * @param distanceDivider Divider of the calculated reference size.
	 * @return Size of the block for adaptive threshold.
	 */
	static int getBlockSize(Size imageSize, double distanceDivider);
};

} /* namespace barcodes */
#endif /* QRDETECTOR_H_ */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       DataSegments.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines DataSegment and DataSegments classes which are
//             returned by decode methods and contains decoded barcode segments.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file DataSegments.h
 *
 * @brief Defines DataSegment and DataSegments classes which are
 *        returned by decode methods and contains decoded barcode segments.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef DATASEGMENT_H_
#define DATASEGMENT_H_

#include <vector>
#include <opencv2/core/core.hpp>

namespace barcodes {
	using namespace cv;
	using namespace std;

	typedef vector<uint8_t> ByteArray;

	/**
	 * Class which holds decoded segment of data.
	 */
	class DataSegment {
	public:
		ByteArray data;			/**< Decoded data */
		int mode;				/**< Mode/Type of which are data */
		int flags;				/**< Some additional flags which informs about decode process of this segment */
		size_t remainderBits;	/**< Bits which are excluded from the data array */

		DataSegment() : mode(0), flags(0), remainderBits(0) {}
	};

	/**
	 * Class which holds all segments of data.
	 *
	 * @see DataSegment
	 */
	class DataSegments:public vector<DataSegment> {
	public:
		static const int DATA_SEGMENTS_CORRUPTED = 0x01; /**< Some segments might be corrupted. */
		int flags;				/**< Flags which informs about how whole decode process finished */

		DataSegments() : flags(0) {}
	};
}

#endif

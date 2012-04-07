///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Barcode.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines Barcode class that only specifies
//             the interface for creation of all barcode classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Barcode.h
 *
 * @brief Defines Barcode class that only specifies
 *        the interface for creation of all barcode classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef BARCODES_BARCODE_H_
#define BARCODES_BARCODE_H_

#include <string>

#include "../common/Image.h"

namespace barcodes {
	/**
	 * Base class for creation of the barcode classes.
	 */
	class Barcode {
	public:
		virtual ~Barcode() {}

		/**
		 * Virtual method which should be used for building of the image with the barcode.
		 * Now returns 0.
		 *
		 * @return Image which contains created barcode.
		 */
		virtual Image *createImage();
	};
}

#endif

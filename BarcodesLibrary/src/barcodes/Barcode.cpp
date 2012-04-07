///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Barcode.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the Barcode class members. Barcode class only specifies
//             the interface for creation of all barcode classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Barcode.cpp
 *
 * @brief Defines the Barcode class members. Barcode class only specifies
 *        the interface for creation of all barcode classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "Barcode.h"

using namespace barcodes;

/**
 * Virtual method which should be used for building of the image with the barcode.
 * Now returns 0.
 *
 * @return Image which contains created barcode.
 */
Image *Barcode::createImage() {
	return 0;
}

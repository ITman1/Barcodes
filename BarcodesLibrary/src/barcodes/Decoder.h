///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       Decoder.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines Decoder class that only specifies the interface for
//             creation of all decoder classes.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file Decoder.h
 *
 * @brief Defines Decoder class that only specifies the interface for
 *        creation of all decoder classes.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef DECODER_H_
#define DECODER_H_

#include "../common/Image.h"
#include "DataSegments.h"

namespace barcodes {

/**
 * Abstract base class for creation of the classes which serves for decoding the barcode
 * from the image and returning its data segments.
 */
class Decoder {
protected:
	Decoder() {}
	virtual ~Decoder() {}
public:
	/**
	 * Decodes barcode on the image and returns decoded data segments.
	 *
	 * @param image Image with the barcode.
	 * @param dataSegments Result decoded data segments.
	 * @param flags Flags which might be used for decode process.
	 */
	virtual void decode(Image &image, DataSegments &dataSegments, int flags = 0) const = 0;
};

} /* namespace barcodes */
#endif /* DECODER_H_ */

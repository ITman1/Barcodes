#ifndef BARCODES_BARCODE_H_
#define BARCODES_BARCODE_H_

#include <string>

#include "../common/Image.h"

namespace barcodes {
	class Barcode {
	public:
		Barcode() {}
		virtual ~Barcode() {}

		virtual Image *createImage();
	};
}

#endif

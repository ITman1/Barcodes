/*
 * types.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_COMMON_H_
#define BARCODES_COMMON_H_

#include <opencv2/core/core.hpp>
#include "common/Image.h"
#include "barcodes/qr/QrBarcode.h"

#include <vector>
#include <cstdarg>

using namespace std;

namespace barcodes {
	using namespace cv;

	template <class T>
	string toStr(const T &value) {
		stringstream ss;
		ss << value;
		return ss.str();
	}
};

#endif /* BARCODES_COMMON_H_ */

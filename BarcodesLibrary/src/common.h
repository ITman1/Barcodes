///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       common.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Includes some common classes or type definitions.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file common.h
 *
 * @brief Includes some common classes or type definitions.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef BARCODES_COMMON_H_
#define BARCODES_COMMON_H_

#include <string>
#include <opencv2/core/core.hpp>

#include "common/Image.h"
#include "barcodes/qr/QrBarcode.h"

using namespace std;

namespace barcodes {
using namespace std;
using namespace cv;

/**
 *
 * @param value
 * @return
 */
template <class T>
string toStr(const T &value) {
	stringstream ss;
	ss << value;
	return ss.str();
}

typedef vector<uint8_t> ByteArray;

};

#endif /* BARCODES_COMMON_H_ */

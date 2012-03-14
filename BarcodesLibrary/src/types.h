/*
 * types.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_TYPES_H_
#define BARCODES_TYPES_H_

#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

namespace barcodes {
	typedef Vec<uchar, 1> ByteArray;

	template <class T>
	string toStr(const T &value) {
		stringstream ss;
		ss << value;
		return ss.str();
	}
};

#endif /* TYPES_H_ */

/*
 * types.h
 *
 *  Created on: 8.3.2012
 *      Author: Scotty
 */

#ifndef BARCODES_TYPES_H_
#define BARCODES_TYPES_H_

#include <opencv2/core/core.hpp>
#include "common/Image.h"
#include "common/Line2D.h"
#include "common/Polygon2D.h"
#include "common/Vector2D.h"
#include "common/miscellaneous.h"

#include <vector>
#include <cstdarg>

#ifdef TARGET_DEBUG
	#ifdef ANDROID
		#include <android/log.h>
		#define DEBUG_TAG "QrBarcode.cpp"

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(string("sdcard/") + string(filename), image)
		#define DEBUG_PRINT(tag, format, ...)	__android_log_print(ANDROID_LOG_DEBUG, string(tag).c_str(), string(format).c_str(), ## __VA_ARGS__)
	#else
		#include <cstdio>

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(filename, image)
		#define DEBUG_PRINT(tag, format, ...) printf((string(tag) + string(": ") + string(format) + string("\n")).c_str(), ## __VA_ARGS__)
	#endif

#else
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_PRINT(tag, text, args...)
#endif

using namespace std;

namespace barcodes {
	using namespace cv;

	typedef vector<uchar> ByteArray;

	template <class T>
	string toStr(const T &value) {
		stringstream ss;
		ss << value;
		return ss.str();
	}
};

#endif /* TYPES_H_ */

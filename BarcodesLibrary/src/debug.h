/*
 * debug.h
 *
 *  Created on: 31.3.2012
 *      Author: Scotty
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef TARGET_DEBUG
	#include <vector>
	#include <sstream>
	#include <string>
	#include <opencv2/core/core.hpp>

	static std::string __DEBUG_ROW_STR;
	static cv::Mat __DEBUG_MATRIX;

	template <class T>
		std::string __DEBUG_TO_STR(const T &value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	#ifdef ANDROID
		#include <android/log.h>

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(std::string("sdcard/") + std::string(filename), image)
		#define DEBUG_PRINT(tag, format, ...)	__android_log_print(ANDROID_LOG_DEBUG, std::string(tag).c_str(), std::string(format).c_str(), ## __VA_ARGS__)
	#else
		#include <cstdio>

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(filename, image)
		#define DEBUG_PRINT(tag, format, ...) printf((std::string(tag) + std::string(": ") + std::string(format) + std::string("\n")).c_str(), ## __VA_ARGS__); fflush(stdout);
	#endif

	#define DEBUG_PRINT_MATRIX(tag, matrix, type) \
		for( int i = 0; i < matrix.rows; i++ ) { \
			for( int j = 0; j < matrix.cols; j++ ) { \
				DEBUG_PRINT(tag, "%d,%d: %d", i, j, matrix.at<type>(i, j)); \
			} \
		}

	#define DEBUG_PRINT_VECTOR(tag, vector) \
		__DEBUG_ROW_STR.clear(); \
		for( unsigned int j = 0; j < vector.size(); j++ ) { \
			__DEBUG_ROW_STR += __DEBUG_TO_STR(vector.at(j)) + std::string(" "); \
		} \
		DEBUG_PRINT(tag, "%s", __DEBUG_ROW_STR.c_str());

	#define DEBUG_PRINT_BITVECTOR(tag, bitvector) \
		__DEBUG_ROW_STR.clear(); \
		for( unsigned int j = 0; j < bitvector.size(); j++ ) { \
			__DEBUG_ROW_STR += (bitvector.at(j))? "1 " : "0 "; \
		} \
		DEBUG_PRINT(tag, "%s", __DEBUG_ROW_STR.c_str());

	#define DEBUG_WRITE_BITMATRIX(filename, bitmatrix) \
		__DEBUG_MATRIX.create(bitmatrix.size(), CV_8UC1); \
		for (int i = 0; i < __DEBUG_MATRIX.rows; i++) { \
			for (int j = 0; j < __DEBUG_MATRIX.cols; j++) { \
				__DEBUG_MATRIX.at<uchar>(i, j) = -255 * (bitmatrix.at(i, j) - 1); \
			} \
		} \
		DEBUG_WRITE_IMAGE(filename, __DEBUG_MATRIX);

#else
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_PRINT(tag, text, args...)
	#define DEBUG_PRINT_MATRIX(tag, matrix, type)
	#define DEBUG_PRINT_VECTOR(tag, vector)
	#define DEBUG_PRINT_BITVECTOR(tag, bitvector)
#endif

#ifndef TARGET_DEBUG_EXTRA
	#undef DEBUG_WRITE_IMAGE
	#undef DEBUG_WRITE_BITMATRIX(filename, bitmatrix)
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_WRITE_BITMATRIX(filename, bitmatrix)
#endif


#endif /* DEBUG_H_ */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       debug.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains macros for debugging. If TARGET_DEBUG is defined,
//             these macros expands to corresponding commands, otherwise no
//             debug commands are expanded. For the expansion of the macros
//             which uses I/O file operations must be defined
//             TARGET_DEBUG_EXTRA.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file debug.h
 *
 * @brief Contains macros for debugging. If TARGET_DEBUG is defined, these
 *        macros expands to corresponding commands, otherwise no debug commands
 *        are expanded. For the expansion of the macros which uses I/O file
 *        operations must be defined TARGET_DEBUG_EXTRA.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef TARGET_DEBUG
	#include <vector>
	#include <sstream>
	#include <string>
	#include <ctime>
	#include <opencv2/core/core.hpp>

	/** Debug string used for string collection. */
	static std::string __DEBUG_ROW_STR;

	/** Matrix used in debug macros. */
	static cv::Mat __DEBUG_MATRIX;

	/**
	 * Converts value to the string.
	 *
	 * @param value Value to be converted into string.
	 */
	template <class T>
	std::string __DEBUG_TO_STR(const T &value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	#ifdef ANDROID
		#include <android/log.h>

		// Saves image into file.
		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(std::string("sdcard/debug_images/") + std::string(filename), image)
		// Prints debug text on stdout.
		#define DEBUG_PRINT(tag, format, ...)	__android_log_print(ANDROID_LOG_DEBUG, std::string(tag).c_str(), std::string(format).c_str(), ## __VA_ARGS__)
	#else
		#include <cstdio>

		#define DEBUG_WRITE_IMAGE(filename, image) imwrite(std::string("debug_images/") + std::string(filename), image)
		#define DEBUG_PRINT(tag, format, ...) printf((std::string(tag) + std::string(": ") + std::string(format) + std::string("\n")).c_str(), ## __VA_ARGS__); fflush(stdout);
	#endif

	// Prints matrix on stdout.
	#define DEBUG_PRINT_MATRIX(tag, matrix, type) \
		for( int i = 0; i < matrix.rows; i++ ) { \
			for( int j = 0; j < matrix.cols; j++ ) { \
				DEBUG_PRINT(tag, "%d,%d: %d", i, j, matrix.at<type>(i, j)); \
			} \
		}

	// Prints vector on stdout.
	#define DEBUG_PRINT_VECTOR(tag, vector) \
		__DEBUG_ROW_STR.clear(); \
		for( unsigned int j = 0; j < vector.size(); j++ ) { \
			__DEBUG_ROW_STR += __DEBUG_TO_STR(vector.at(j)) + std::string(" "); \
		} \
		DEBUG_PRINT(tag, "%s", __DEBUG_ROW_STR.c_str());

	// Prints bit vector on stdout.
	#define DEBUG_PRINT_BITVECTOR(tag, bitvector) \
		__DEBUG_ROW_STR.clear(); \
		for( unsigned int j = 0; j < bitvector.size(); j++ ) { \
			__DEBUG_ROW_STR += (bitvector.at(j))? "1 " : "0 "; \
		} \
		DEBUG_PRINT(tag, "%s", __DEBUG_ROW_STR.c_str());

	// Saves bit matrix into file.
	#define DEBUG_WRITE_BITMATRIX(filename, bitmatrix) \
		__DEBUG_MATRIX.create(bitmatrix.size(), CV_8UC1); \
		for (int i = 0; i < __DEBUG_MATRIX.rows; i++) { \
			for (int j = 0; j < __DEBUG_MATRIX.cols; j++) { \
				__DEBUG_MATRIX.at<uchar>(i, j) = -255 * (bitmatrix.at(i, j) - 1); \
			} \
		} \
		DEBUG_WRITE_IMAGE(filename, __DEBUG_MATRIX);

	static clock_t __clock_m = clock();
	static clock_t __clock_diff;
	#define DIFF_TIME() \
		((__clock_diff = clock() - __clock_m, __clock_m += __clock_diff), (int)(1000 * ((double)__clock_diff / (double)CLOCKS_PER_SEC)))

#else
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_PRINT(tag, text, args...)
	#define DEBUG_PRINT_MATRIX(tag, matrix, type)
	#define DEBUG_PRINT_VECTOR(tag, vector)
	#define DEBUG_PRINT_BITVECTOR(tag, bitvector)
#endif

#ifndef TARGET_DEBUG_EXTRA
	#undef DEBUG_WRITE_IMAGE
	#undef DEBUG_WRITE_BITMATRIX
	#define DEBUG_WRITE_IMAGE(filename, image)
	#define DEBUG_WRITE_BITMATRIX(filename, bitmatrix)
#endif

#endif /* DEBUG_H_ */

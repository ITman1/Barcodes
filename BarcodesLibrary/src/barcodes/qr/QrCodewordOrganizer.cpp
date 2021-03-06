///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrCodewordOrganizer.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrCodewordOrganizer class that implements
//             extracting methods of the data/error correction blocks/codewords.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrCodewordOrganizer.cpp
 *
 * @brief DDefines members of QrCodewordOrganizer class that implements
 *        extracting methods of the data/error correction blocks/codewords.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <cstdarg>

#include "QrCodewordOrganizer.h"
#include "QrReedSolomon.h"
#include "../common/BitStream.h"

namespace barcodes {

/**
 * Constructs characteristics for specified number of records.
 *
 * @param count Number of characteristics records.
 */
QrVersionFormatCharacteristics::QrVersionFormatCharacteristics(int count, ...) {
	va_list args;
	va_start(args, count);
	insertCharacteristics(count,  args);
	va_end(args);
}

/**
 * Constructs characteristics only with one record.
 *
 * @param first Record of the characteristics.
 */
QrVersionFormatCharacteristics::QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first) {
	insertCharacteristics(1, &first);
}

/**
 * Constructs characteristics only with two records.
 *
 * @param first First record of the characteristics.
 * @param second Second record of the characteristics.
 */
QrVersionFormatCharacteristics::QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first,
		QrVersionFormatCharacteristic second) {
	insertCharacteristics(2, &first, &second);
}

/**
 * Inserts characteristics into vector.
 *
 * @param count Number of characteristics records.
 */
void QrVersionFormatCharacteristics::insertCharacteristics(int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		QrVersionFormatCharacteristic *obj = va_arg(args, QrVersionFormatCharacteristic *);
		push_back(*obj);
	}

	va_end(args);
}

#define _VERSION_FORMAT(num, format) make_pair(make_pair(num, QrFormatInformation::\
		ERROR_CORRECT_LEVEL_ ## format), QrVersionFormatCharacteristics(
#define _CH(a,b,c,d,e) QrVersionFormatCharacteristic(a,b,c,d,e)
#define _CH_END ))

/**
 * Helper array for the characteristics map table.
 */
const pair<QrCodewordOrganizer::MAP_KEY_TYPE, QrVersionFormatCharacteristics>
	QrCodewordOrganizer::QrCodewordOrganizer_mapping[] = {
	_VERSION_FORMAT(1, L)  _CH(3, 1,  26,  19,  2)   _CH_END,
	_VERSION_FORMAT(1, M)  _CH(2, 1,  26,  16,  4)   _CH_END,
	_VERSION_FORMAT(1, Q)  _CH(1, 1,  26,  13,  6)   _CH_END,
	_VERSION_FORMAT(1, H)  _CH(1, 1,  26,  9,   8)   _CH_END,

	_VERSION_FORMAT(2, L)  _CH(2, 1,  44,  34,  4)   _CH_END,
	_VERSION_FORMAT(2, M)  _CH(0, 1,  44,  28,  8)   _CH_END,
	_VERSION_FORMAT(2, Q)  _CH(0, 1,  44,  22,  11)  _CH_END,
	_VERSION_FORMAT(2, H)  _CH(0, 1,  44,  16,  14)  _CH_END,

	_VERSION_FORMAT(3, L)  _CH(1, 1,  70,  55,  7)   _CH_END,
	_VERSION_FORMAT(3, M)  _CH(0, 1,  70,  44,  13)  _CH_END,
	_VERSION_FORMAT(3, Q)  _CH(0, 2,  35,  17,  9)   _CH_END,
	_VERSION_FORMAT(3, H)  _CH(0, 2,  35,  13,  11)  _CH_END,

	_VERSION_FORMAT(4, L)  _CH(0, 1,  100, 80,  10)  _CH_END,
	_VERSION_FORMAT(4, M)  _CH(0, 2,  50,  32,  9)   _CH_END,
	_VERSION_FORMAT(4, Q)  _CH(0, 2,  50,  24,  13)  _CH_END,
	_VERSION_FORMAT(4, H)  _CH(0, 4,  25,  9,   8)   _CH_END,

	_VERSION_FORMAT(5, L)  _CH(0, 1,  134, 108, 13)  _CH_END,
	_VERSION_FORMAT(5, M)  _CH(0, 2,  67,  43,  12)  _CH_END,
	_VERSION_FORMAT(5, Q)  _CH(0, 2,  33,  15,  9),  _CH(0, 2,  34,  16,  9)   _CH_END,
	_VERSION_FORMAT(5, H)  _CH(0, 2,  33,  11,  11), _CH(0, 2,  34,  12,  11)  _CH_END,

	_VERSION_FORMAT(6, L)  _CH(0, 2,  86,  68,  9)   _CH_END,
	_VERSION_FORMAT(6, M)  _CH(0, 4,  43,  27,  8)   _CH_END,
	_VERSION_FORMAT(6, Q)  _CH(0, 4,  43,  19,  12)  _CH_END,
	_VERSION_FORMAT(6, H)  _CH(0, 4,  43,  15,  14)  _CH_END,

	_VERSION_FORMAT(7, L)  _CH(0, 2,  98,  78,  10)  _CH_END,
	_VERSION_FORMAT(7, M)  _CH(0, 4,  49,  31,  9)   _CH_END,
	_VERSION_FORMAT(7, Q)  _CH(0, 2,  32,  14,  9),  _CH(0, 4,  33,  15,  9)   _CH_END,
	_VERSION_FORMAT(7, H)  _CH(0, 4,  39,  13,  13), _CH(0, 1,  40,  14,  13)  _CH_END,

	_VERSION_FORMAT(8, L)  _CH(0, 2,  121, 97,  12)  _CH_END,
	_VERSION_FORMAT(8, M)  _CH(0, 2,  60,  38,  11), _CH(0, 2,  61,  39,  11)  _CH_END,
	_VERSION_FORMAT(8, Q)  _CH(0, 4,  40,  18,  11), _CH(0, 2,  41,  19,  11)  _CH_END,
	_VERSION_FORMAT(8, H)  _CH(0, 4,  40,  14,  13), _CH(0, 2,  41,  15,  13)  _CH_END,

	_VERSION_FORMAT(9, L)  _CH(0, 2,  146, 116, 15)  _CH_END,
	_VERSION_FORMAT(9, M)  _CH(0, 3,  58,  36,  11), _CH(0, 2,  59,  37,  11)  _CH_END,
	_VERSION_FORMAT(9, Q)  _CH(0, 4,  36,  16,  10), _CH(0, 4,  37,  17,  10)  _CH_END,
	_VERSION_FORMAT(9, H)  _CH(0, 4,  36,  12,  12), _CH(0, 4,  37,  13,  12)  _CH_END,

	_VERSION_FORMAT(10, L) _CH(0, 2,  86,  68,  9),  _CH(0, 2,  87,  69,  9)   _CH_END,
	_VERSION_FORMAT(10, M) _CH(0, 4,  69,  43,  13), _CH(0, 1,  70,  44,  13)  _CH_END,
	_VERSION_FORMAT(10, Q) _CH(0, 6,  43,  19,  12), _CH(0, 2,  44,  20,  12)  _CH_END,
	_VERSION_FORMAT(10, H) _CH(0, 6,  43,  15,  14), _CH(0, 2,  44,  16,  14)  _CH_END,

	_VERSION_FORMAT(11, L) _CH(0, 4,  101, 81,  10)  _CH_END,
	_VERSION_FORMAT(11, M) _CH(0, 1,  80,  50,  15), _CH(0, 4,  81,  51,  15)  _CH_END,
	_VERSION_FORMAT(11, Q) _CH(0, 4,  50,  22,  14), _CH(0, 4,  51,  23,  14)  _CH_END,
	_VERSION_FORMAT(11, H) _CH(0, 3,  36,  12,  12), _CH(0, 8,  37,  13,  12)  _CH_END,

	_VERSION_FORMAT(12, L) _CH(0, 2,  116, 92,  12), _CH(0, 2,  117, 93,  12)  _CH_END,
	_VERSION_FORMAT(12, M) _CH(0, 6,  58,  36,  11), _CH(0, 2,  59,  37,  11)  _CH_END,
	_VERSION_FORMAT(12, Q) _CH(0, 4,  46,  20,  13), _CH(0, 6,  47,  21,  13)  _CH_END,
	_VERSION_FORMAT(12, H) _CH(0, 7,  42,  14,  14), _CH(0, 4,  43,  15,  14)  _CH_END,

	_VERSION_FORMAT(13, L) _CH(0, 4,  133, 107, 13)  _CH_END,
	_VERSION_FORMAT(13, M) _CH(0, 8,  59,  37,  11), _CH(0, 1,  60,  38,  11)  _CH_END,
	_VERSION_FORMAT(13, Q) _CH(0, 8,  44,  20,  12), _CH(0, 4,  45,  21,  12)  _CH_END,
	_VERSION_FORMAT(13, H) _CH(0, 12, 33,  11,  11), _CH(0, 4,  34,  12,  11)  _CH_END,

	_VERSION_FORMAT(14, L) _CH(0, 3,  145, 115, 15), _CH(0, 1,  146, 116, 15)  _CH_END,
	_VERSION_FORMAT(14, M) _CH(0, 4,  64,  40,  12), _CH(0, 5,  65,  41,  12)  _CH_END,
	_VERSION_FORMAT(14, Q) _CH(0, 11, 36,  16,  10), _CH(0, 5,  37,  17,  10)  _CH_END,
	_VERSION_FORMAT(14, H) _CH(0, 11, 36,  12,  12), _CH(0, 5,  37,  13,  12)  _CH_END,

	_VERSION_FORMAT(15, L) _CH(0, 5,  109, 87,  11), _CH(0, 1,  110, 88,  11)  _CH_END,
	_VERSION_FORMAT(15, M) _CH(0, 5,  65,  41,  12), _CH(0, 5,  66,  42,  12)  _CH_END,
	_VERSION_FORMAT(15, Q) _CH(0, 5,  54,  24,  15), _CH(0, 7,  55,  25,  15)  _CH_END,
	_VERSION_FORMAT(15, H) _CH(0, 11, 36,  12,  12), _CH(0, 7,  37,  13,  12)  _CH_END,

	_VERSION_FORMAT(16, L) _CH(0, 5,  122, 98,  12), _CH(0, 1,  123, 99,  12)  _CH_END,
	_VERSION_FORMAT(16, M) _CH(0, 7,  73,  45,  14), _CH(0, 3,  74,  46,  14)  _CH_END,
	_VERSION_FORMAT(16, Q) _CH(0, 15, 43,  19,  12), _CH(0, 2,  44,  20,  12)  _CH_END,
	_VERSION_FORMAT(16, H) _CH(0, 3,  45,  15,  15), _CH(0, 13, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(17, L) _CH(0, 1,  135, 107, 14), _CH(0, 5,  136, 108, 14)  _CH_END,
	_VERSION_FORMAT(17, M) _CH(0, 10, 74,  46,  14), _CH(0, 1,  75,  47,  14)  _CH_END,
	_VERSION_FORMAT(17, Q) _CH(0, 1,  50,  22,  14), _CH(0, 15, 51,  23,  14)  _CH_END,
	_VERSION_FORMAT(17, H) _CH(0, 2,  42,  14,  14), _CH(0, 17, 43,  15,  14)  _CH_END,

	_VERSION_FORMAT(18, L) _CH(0, 5,  150, 120, 15), _CH(0, 1,  151, 121, 15)  _CH_END,
	_VERSION_FORMAT(18, M) _CH(0, 9,  69,  43,  13), _CH(0, 4,  70,  44,  13)  _CH_END,
	_VERSION_FORMAT(18, Q) _CH(0, 17, 50,  22,  14), _CH(0, 1,  51,  23,  14)  _CH_END,
	_VERSION_FORMAT(18, H) _CH(0, 2,  42,  14,  14), _CH(0, 19, 43,  15,  14)  _CH_END,

	_VERSION_FORMAT(19, L) _CH(0, 3,  141, 113, 14), _CH(0, 4,  142, 114, 14)  _CH_END,
	_VERSION_FORMAT(19, M) _CH(0, 3,  70,  44,  13), _CH(0, 11, 71,  45,  13)  _CH_END,
	_VERSION_FORMAT(19, Q) _CH(0, 17, 47,  21,  13), _CH(0, 4,  48,  22,  13)  _CH_END,
	_VERSION_FORMAT(19, H) _CH(0, 9,  39,  13,  13), _CH(0, 16, 40,  14,  13)  _CH_END,

	_VERSION_FORMAT(20, L) _CH(0, 3,  135, 107, 14), _CH(0, 5,  136, 108, 14)  _CH_END,
	_VERSION_FORMAT(20, M) _CH(0, 3,  67,  41,  13), _CH(0, 13, 68,  42,  13)  _CH_END,
	_VERSION_FORMAT(20, Q) _CH(0, 15, 54,  24,  15), _CH(0, 5,  55,  25,  15)  _CH_END,
	_VERSION_FORMAT(20, H) _CH(0, 15, 43,  15,  14), _CH(0, 10, 44,  16,  14)  _CH_END,

	_VERSION_FORMAT(21, L) _CH(0, 4,  144, 116, 14), _CH(0, 4,  145, 117, 14)  _CH_END,
	_VERSION_FORMAT(21, M) _CH(0, 17, 68,  42,  13)  _CH_END,
	_VERSION_FORMAT(21, Q) _CH(0, 17, 50,  22,  14), _CH(0, 6,  51,  23,  14)  _CH_END,
	_VERSION_FORMAT(21, H) _CH(0, 19, 46,  16,  15), _CH(0, 6,  47,  17,  15)  _CH_END,

	_VERSION_FORMAT(22, L) _CH(0, 2,  139, 111, 14), _CH(0, 7,  140, 112, 14)  _CH_END,
	_VERSION_FORMAT(22, M) _CH(0, 17, 74,  46,  14)  _CH_END,
	_VERSION_FORMAT(22, Q) _CH(0, 7,  54,  24,  15), _CH(0, 16, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(22, H) _CH(0, 34, 37,  13,  12)  _CH_END,

	_VERSION_FORMAT(23, L) _CH(0, 4,  151, 121, 15), _CH(0, 5,  152, 122, 15)  _CH_END,
	_VERSION_FORMAT(23, M) _CH(0, 4,  75,  47,  14), _CH(0, 14, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(23, Q) _CH(0, 11, 54,  24,  15), _CH(0, 14, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(23, H) _CH(0, 16, 45,  15,  15), _CH(0, 14, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(24, L) _CH(0, 6,  147, 117, 15), _CH(0, 4,  148, 118, 15)  _CH_END,
	_VERSION_FORMAT(24, M) _CH(0, 6,  73,  45,  14), _CH(0, 14, 74,  46,  14)  _CH_END,
	_VERSION_FORMAT(24, Q) _CH(0, 11, 54,  24,  15), _CH(0, 16, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(24, H) _CH(0, 30, 46,  16,  15), _CH(0, 2,  47,  17,  15)  _CH_END,

	_VERSION_FORMAT(25, L) _CH(0, 8,  132, 106, 13), _CH(0, 4,  133, 107, 13)  _CH_END,
	_VERSION_FORMAT(25, M) _CH(0, 8,  75,  47,  14), _CH(0, 13, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(25, Q) _CH(0, 7,  54,  24,  15), _CH(0, 22, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(25, H) _CH(0, 22, 45,  15,  15), _CH(0, 13, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(26, L) _CH(0, 10, 142, 114, 14), _CH(0, 2,  143, 115, 14)  _CH_END,
	_VERSION_FORMAT(26, M) _CH(0, 19, 74,  46,  14), _CH(0, 4,  75,  47,  14)  _CH_END,
	_VERSION_FORMAT(26, Q) _CH(0, 28, 50,  22,  14), _CH(0, 6,  51,  23,  14)  _CH_END,
	_VERSION_FORMAT(26, H) _CH(0, 33, 46,  16,  15), _CH(0, 4,  47,  17,  15)  _CH_END,

	_VERSION_FORMAT(27, L) _CH(0, 8,  152, 122, 15), _CH(0, 4,  153, 123, 15)  _CH_END,
	_VERSION_FORMAT(27, M) _CH(0, 22, 73,  45,  14), _CH(0, 3,  74,  46,  14)  _CH_END,
	_VERSION_FORMAT(27, Q) _CH(0, 8,  53,  23,  15), _CH(0, 26, 54,  24,  15)  _CH_END,
	_VERSION_FORMAT(27, H) _CH(0, 12, 45,  15,  15), _CH(0, 28, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(28, L) _CH(0, 3,  147, 117, 15), _CH(0, 10, 148, 118, 15)  _CH_END,
	_VERSION_FORMAT(28, M) _CH(0, 3,  73,  45,  14), _CH(0, 23, 74,  46,  14)  _CH_END,
	_VERSION_FORMAT(28, Q) _CH(0, 4,  54,  24,  15), _CH(0, 31, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(28, H) _CH(0, 11, 45,  15,  15), _CH(0, 31, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(29, L) _CH(0, 7,  146, 116, 15), _CH(0, 7,  147, 117, 15)  _CH_END,
	_VERSION_FORMAT(29, M) _CH(0, 21, 73,  45,  14), _CH(0, 7,  74,  46,  14)  _CH_END,
	_VERSION_FORMAT(29, Q) _CH(0, 1,  53,  23,  15), _CH(0, 37, 54,  24,  15)  _CH_END,
	_VERSION_FORMAT(29, H) _CH(0, 19, 45,  15,  15), _CH(0, 26, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(30, L) _CH(0, 5,  145, 115, 15), _CH(0, 10, 146, 116, 15)  _CH_END,
	_VERSION_FORMAT(30, M) _CH(0, 19, 75,  47,  14), _CH(0, 10, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(30, Q) _CH(0, 15, 54,  24,  15), _CH(0, 25, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(30, H) _CH(0, 23, 45,  15,  15), _CH(0, 25, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(31, L) _CH(0, 13, 145, 115, 15), _CH(0, 3,  146, 116, 15)  _CH_END,
	_VERSION_FORMAT(31, M) _CH(0, 2,  74,  46,  14), _CH(0, 29, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(31, Q) _CH(0, 42, 54,  24,  15), _CH(0, 1,  55,  25,  15)  _CH_END,
	_VERSION_FORMAT(31, H) _CH(0, 23, 45,  15,  15), _CH(0, 28, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(32, L) _CH(0, 17, 145, 115, 15)  _CH_END,
	_VERSION_FORMAT(32, M) _CH(0, 10, 74,  46,  14), _CH(0, 23, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(32, Q) _CH(0, 10, 54,  24,  15), _CH(0, 35, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(32, H) _CH(0, 19, 45,  15,  15), _CH(0, 35, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(33, L) _CH(0, 17, 145, 115, 15), _CH(0, 1,  146, 116, 15)  _CH_END,
	_VERSION_FORMAT(33, M) _CH(0, 14, 74,  46,  14), _CH(0, 21, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(33, Q) _CH(0, 29, 54,  24,  15), _CH(0, 19, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(33, H) _CH(0, 11, 45,  15,  15), _CH(0, 46, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(34, L) _CH(0, 13, 145, 115, 15), _CH(0, 6,  146, 116, 15)  _CH_END,
	_VERSION_FORMAT(34, M) _CH(0, 14, 74,  46,  14), _CH(0, 23, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(34, Q) _CH(0, 44, 54,  24,  15), _CH(0, 7,  55,  25,  15)  _CH_END,
	_VERSION_FORMAT(34, H) _CH(0, 59, 46,  16,  15), _CH(0, 1,  47,  17,  15)  _CH_END,

	_VERSION_FORMAT(35, L) _CH(0, 12, 151, 121, 15), _CH(0, 7,  152, 122, 15)  _CH_END,
	_VERSION_FORMAT(35, M) _CH(0, 12, 75,  47,  14), _CH(0, 26, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(35, Q) _CH(0, 39, 54,  24,  15), _CH(0, 14, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(35, H) _CH(0, 22, 45,  15,  15), _CH(0, 41, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(36, L) _CH(0, 6,  151, 121, 15), _CH(0, 14, 152, 122, 15)  _CH_END,
	_VERSION_FORMAT(36, M) _CH(0, 6,  75,  47,  14), _CH(0, 34, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(36, Q) _CH(0, 46, 54,  24,  15), _CH(0, 10, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(36, H) _CH(0, 2,  45,  15,  15), _CH(0, 64, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(37, L) _CH(0, 17, 152, 122,15),  _CH(0, 4,  153, 123, 15)  _CH_END,
	_VERSION_FORMAT(37, M) _CH(0, 29, 74,  46, 14),  _CH(0, 14, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(37, Q) _CH(0, 49, 54,  24, 15),  _CH(0, 10, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(37, H) _CH(0, 24, 45,  15, 15),  _CH(0, 46, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(38, L) _CH(0, 4,  152, 122, 15), _CH(0, 18, 153, 123, 15)  _CH_END,
	_VERSION_FORMAT(38, M) _CH(0, 13, 74,  46,  14), _CH(0, 32, 75,  47,  14)  _CH_END,
	_VERSION_FORMAT(38, Q) _CH(0, 48, 54,  24,  15), _CH(0, 14, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(38, H) _CH(0, 42, 45,  15,  15), _CH(0, 32, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(39, L) _CH(0, 20, 147, 117, 15), _CH(0, 4,  148, 118, 15)  _CH_END,
	_VERSION_FORMAT(39, M) _CH(0, 40, 75,  47,  14), _CH(0, 7,  76,  48,  14)  _CH_END,
	_VERSION_FORMAT(39, Q) _CH(0, 43, 54,  24,  15), _CH(0, 22, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(39, H) _CH(0, 10, 45,  15,  15), _CH(0, 67, 46,  16,  15)  _CH_END,

	_VERSION_FORMAT(40, L) _CH(0, 19, 148, 118, 15), _CH(0, 6,  149, 119, 15)  _CH_END,
	_VERSION_FORMAT(40, M) _CH(0, 18, 75,  47,  14), _CH(0, 31, 76,  48,  14)  _CH_END,
	_VERSION_FORMAT(40, Q) _CH(0, 34, 54,  24,  15), _CH(0, 34, 55,  25,  15)  _CH_END,
	_VERSION_FORMAT(40, H) _CH(0, 20, 45,  15,  15), _CH(0, 61, 46,  16,  15)  _CH_END
};

/**
 * Map array which maps the characteristics for specified version and format of the QR code.
 */
const map<QrCodewordOrganizer::MAP_KEY_TYPE, QrVersionFormatCharacteristics,
	QrCodewordOrganizer::cmp_key> QrCodewordOrganizer::CODEWORD_CHARACTERISTICS(
		QrCodewordOrganizer_mapping,	QrCodewordOrganizer_mapping +
		sizeof QrCodewordOrganizer_mapping / sizeof QrCodewordOrganizer_mapping[0]);


/**
 * Constructs characteristics for passed version and format.
 *
 * @param version Version of the QR code.
 * @param format Format of the QR code.
 */
QrCodewordOrganizer::QrCodewordOrganizer(QrVersionInformation version, QrFormatInformation format)
	: version(version), format(format) {

	getCharacteristics(characteristics);
	codewordSize = version.getCodewordSize();
}

/**
 * Returns characteristics for this version and format of the QR code.
 *
 * @param characteristics Characteristics for this format and version of the QR code.
 */
void QrCodewordOrganizer::getCharacteristics(QrVersionFormatCharacteristics &characteristics) {
	characteristics.clear();
	MAP_KEY_TYPE key(version.getVersion(), format.getErrorCorrectionLevel());

	if (CODEWORD_CHARACTERISTICS.find(key) != CODEWORD_CHARACTERISTICS.end()) {
		characteristics = CODEWORD_CHARACTERISTICS.at(key);
	}
}

/**
 * Extracts data codewords from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted data codewords.
 * @param extractedData Extracted data codewords.
 */
void QrCodewordOrganizer::extractDataCodewords(BitArray &code, BitArray &extractedData) {
	extractedData.clear();

	vector<BitArray> blocks;
	extractDataBlocks(code, blocks);

	dataBlocksToDataCodewords(blocks, extractedData);
}

/**
 * Extracts error correction codewords from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted error correction codewords.
 * @param ecCodewords Extracted error correction codewords.
 */
void QrCodewordOrganizer::extractErrorCorrectionCodewords(BitArray &code, BitArray &ecCodewords) {
	ecCodewords.clear();

	vector<BitArray> blocks;
	extractErrorCorrectionBlocks(code, blocks);

	errorCorrectionBlocksToErrorCorrectionCodewords(blocks, ecCodewords);
}

/**
 * Extracts codewords from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted codewords.
 * @param codewords Extracted codewords.
 */
void QrCodewordOrganizer::extractCodewords(BitArray &code, BitArray &codewords) {
	BitArray ec;

	extractDataCodewords(code, codewords);
	extractErrorCorrectionCodewords(code, ec);

	codewords.insert(codewords.end(), ec.begin(), ec.end());
}

/**
 * Extracts data blocks from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted data blocks.
 * @param blocks Extracted data blocks.
 */
void QrCodewordOrganizer::extractDataBlocks(BitArray &code, vector<BitArray> &blocks) {
	blocks.clear();

	unsigned int errCorrBlocks = 0;
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		errCorrBlocks += characteristics[i].errCorrBlocks;
	}

	unsigned int offset = 0;
	unsigned int j = 0;
	unsigned int offsetBlocks = 0;
	blocks.resize(errCorrBlocks);
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		unsigned int dataBlockSize = characteristics[i].k;

		if (offset + errCorrBlocks * dataBlockSize * codewordSize > code.size()) {
			blocks.clear();
			return;
		}

		for (; j < dataBlockSize; j++) {
			for (unsigned int k = 0; k < errCorrBlocks; k++) {
				BitArray::iterator codewordIter = code.begin() + (codewordSize * offset);
				blocks[offsetBlocks + k].insert(blocks[offsetBlocks + k].end(), codewordIter, codewordIter + codewordSize);
				offset += 1;
			}
		}
		errCorrBlocks -= characteristics[i].errCorrBlocks;
		offsetBlocks += characteristics[i].errCorrBlocks;
	}
}

/**
 * Extracts error correction blocks from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted error correction blocks.
 * @param blocks Extracted error correction blocks.
 */
void QrCodewordOrganizer::extractErrorCorrectionBlocks(BitArray &code, vector<BitArray> &blocks) {
	blocks.clear();

	unsigned int offset = 0;
	unsigned int errCorrBlocks = 0;
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		errCorrBlocks += characteristics[i].errCorrBlocks;
		offset += characteristics[i].errCorrBlocks * characteristics[i].k;
	}

	unsigned int j = 0;
	unsigned int offsetBlocks = 0;
	blocks.resize(errCorrBlocks);
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		unsigned int ecSize = characteristics[i].c - characteristics[i].k;

		if (offset + errCorrBlocks * ecSize * codewordSize > code.size()) {
			blocks.clear();
			return;
		}

		for (; j < ecSize; j++) {
			for (unsigned int k = 0; k < errCorrBlocks; k++) {
				BitArray::iterator codewordIter = code.begin() + (codewordSize * offset);
				blocks[offsetBlocks + k].insert(blocks[offsetBlocks + k].end(), codewordIter, codewordIter + codewordSize);
				offset += 1;
			}
		}
		errCorrBlocks -= characteristics[i].errCorrBlocks;
		offsetBlocks += characteristics[i].errCorrBlocks;
	}
}

/**
 * Extracts blocks from the code.
 *
 * @param code Bit array of the decoded code from which should be extracted blocks.
 * @param blocks Extracted blocks.
 */
void QrCodewordOrganizer::extractBlocks(BitArray &code, vector<BitArray> &blocks) {
	vector<BitArray> ec_blocks;

	extractDataBlocks(code, blocks);
	extractErrorCorrectionBlocks(code, ec_blocks);

	vector<BitArray>::iterator iter = blocks.begin();
	vector<BitArray>::iterator iter2 = ec_blocks.begin();

	for (; iter != blocks.end();iter++, iter2++) {
		iter->insert(iter->end(), iter2->begin(), iter2->end());
	}
}

/**
 * Converts data blocks to data codewords.
 *
 * @param blocks Data blocks.
 * @param dataCodewords Data codewords.
 */
void QrCodewordOrganizer::dataBlocksToDataCodewords(vector<BitArray> &blocks, BitArray &dataCodewords) {
	dataCodewords.clear();
	for (unsigned int i = 0; i < blocks.size(); i++) {
		dataCodewords.insert(dataCodewords.end(), blocks[i].begin(), blocks[i].end());
	}
}

/**
 * Converts error correction blocks to error correction codewords.
 *
 * @param blocks Error correction blocks.
 * @param ecCodewords Error correction codewords.
 */
void QrCodewordOrganizer::errorCorrectionBlocksToErrorCorrectionCodewords(vector<BitArray> &blocks, BitArray &ecCodewords) {
	ecCodewords.clear();
	for (unsigned int i = 0; i < blocks.size(); i++) {
		ecCodewords.insert(ecCodewords.end(), blocks[i].begin(), blocks[i].end());
	}
}

/**
 * Converts blocks to codewords.
 *
 * @param blocks Blocks to be converted.
 * @param codewords Result Codewords.
 */
void QrCodewordOrganizer::blocksToCodewords(vector<BitArray> &blocks, BitArray &codewords) {
	codewords.clear();

	int block = 0;
	BitArray ecCodewords;
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		for (int j = 0; j < characteristics[i].errCorrBlocks; j++) {
			if (codewordSize * characteristics[i].c <= (int)blocks[block].size() ) {
				codewords.insert(codewords.end(), blocks[block].begin(), blocks[block].begin() + codewordSize * characteristics[i].k);
				ecCodewords.insert(ecCodewords.end(), blocks[block].begin() + codewordSize * characteristics[i].k, blocks[block].begin() + codewordSize * characteristics[i].c);
			}
			block++;
		}
	}

	codewords.insert(codewords.end(), ecCodewords.begin(), ecCodewords.end());
}

/**
 * Corrects the blocks.
 *
 * @param blocks Blocks to be corrected.
 * @return True if no correction was applied or correction finished with success, otherwise false.
 */
bool QrCodewordOrganizer::correctBlocks(vector<BitArray> &blocks) {
	int block = 0;
	vector<int> vec;
	bool res = true;
	vector<BitArray> _blocks;
	BitArray bitArray;
	for (unsigned int i = 0; i < characteristics.size(); i++) {
		QrReedSolomon reedSolomon(characteristics[i].c - characteristics[i].k);
		for (int j = 0; j < characteristics[i].errCorrBlocks; j++) {
			if (codewordSize * characteristics[i].c <= (int)blocks[block].size() ) {
				bitArrayToCodewordArray(blocks[block], vec);
				if (reedSolomon.correct(vec)) {
					codewordArrayToBitArray(vec, bitArray);
					_blocks.push_back(bitArray);
				} else {
					res = false;
					_blocks.push_back(blocks[block]);
				}
			}
			block++;
		}
	}

	blocks = _blocks;
	return res;
}

/**
 * Converts bit array to the byte array.
 *
 * @param bitArray Bit array to be converted.
 * @param vec Vector which holds byte array.
 */
void QrCodewordOrganizer::bitArrayToCodewordArray(BitArray &bitArray, vector<int> &vec) {
	vec.clear();
	BitStreamReverseCodeword <int> bitsStream(bitArray);

	uchar byte = 0;
	while (!bitsStream.isEnd()) {
		bitsStream(codewordSize) >> byte;
		vec.push_back(byte);
	}
}

/**
 * Converts byte array to the vector.
 *
 * @param vec Vector that holds byte array.
 * @param bitArray Output bit array.
 */
void QrCodewordOrganizer::codewordArrayToBitArray(vector<int> &vec, BitArray &bitArray) {
	bitArray.clear();

	vector<int>::iterator iter = vec.begin();
	for (;iter != vec.end(); iter++) {
		bitArray.pushNumber((uchar)*iter, 8);
	}
}

} /* namespace barcodes */

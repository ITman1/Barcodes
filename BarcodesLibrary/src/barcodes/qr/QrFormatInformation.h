/*
 * QrFormatInformation.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#ifndef QRFORMATINFORMATION_H_
#define QRFORMATINFORMATION_H_

#include <opencv2/core/core.hpp>

#include "../DetectedMarks.h"
#include "../common/GridSampler.h"
#include "QrVersionInformation.h"

namespace barcodes {
using namespace cv;

class QrFormatInformation {
public:
	typedef enum error_correction_levels {
		ERROR_CORRECT_LEVEL_L = 0x1,
		ERROR_CORRECT_LEVEL_M = 0x2,
		ERROR_CORRECT_LEVEL_Q = 0x3,
		ERROR_CORRECT_LEVEL_H = 0x4
	} ErrorCorrectionLevel;

	typedef enum xor_data_masks {
		XOR_DATA_MASK_000 = 0x0,
		XOR_DATA_MASK_001 = 0x1,
		XOR_DATA_MASK_010 = 0x2,
		XOR_DATA_MASK_011 = 0x3,
		XOR_DATA_MASK_100 = 0x4,
		XOR_DATA_MASK_101 = 0x5,
		XOR_DATA_MASK_110 = 0x6,
		XOR_DATA_MASK_111 = 0x7
	} XORDataMask;

	QrFormatInformation(ErrorCorrectionLevel errorCorrectionLevel, XORDataMask xorDataMask) :
		errorCorrectionLevel(errorCorrectionLevel), xorDataMask(xorDataMask) {}
	virtual ~QrFormatInformation() {}

	void buildXORDataMask(BitMatrix &mask, QrVersionInformation version);

	static QrFormatInformation fromBitMatrix(const BitMatrix &code, QrVersionInformation version);
private:
	const static map<uint32_t, uint32_t> ENCODED_FORMATS;
	ErrorCorrectionLevel errorCorrectionLevel;
	XORDataMask xorDataMask;
};

} /* namespace barcodes */
#endif /* QRFORMATINFORMATION_H_ */

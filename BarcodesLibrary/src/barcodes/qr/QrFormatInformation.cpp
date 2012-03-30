/*
 * QrFormatInformation.cpp
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
 */

#include "QrFormatInformation.h"

namespace barcodes {

class XORDataMaskCalcFunctor {
public:
	virtual ~XORDataMaskCalcFunctor() {}
	inline virtual bool calc(int i, int j) = 0;
};

class XORDataMaskCalc000: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i + j) % 2 == 0;
	}
};

class XORDataMaskCalc001: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return i % 2 == 0;
	}
};

class XORDataMaskCalc010: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return j % 3 == 0;
	}
};

class XORDataMaskCalc011: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i + j) % 3 == 0;
	}
};

class XORDataMaskCalc100: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i / 2) + (j / 3)) % 2 == 0;
	}
};

class XORDataMaskCalc101: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i * j) % 2 + (i * j) % 3 == 0;
	}
};

class XORDataMaskCalc110: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i * j) % 2 + (i * j) % 3) % 2 == 0;
	}
};

class XORDataMaskCalc111: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i + j) % 2 + (i * j) % 3) % 2 == 0;
	}
};

template<class CalcFunctor>
void _buildXORDataMask(BitMatrix &mask, Size &size) {
	mask = BitMatrix(size);
	CalcFunctor calc;
	for( int row = 0; row < mask.rows; row++ ) {
	    for( int col = 0; col < mask.cols; col++ ) {
	    	mask.setBit(row, col, calc.calc(row, col));
	    }
	}
}

pair<uint32_t, uint32_t> QrFormatInformation_formats_mapping[] = {
	make_pair(0x5412, 0x00),	make_pair(0x5125, 0x01),
	make_pair(0x5E7C, 0x02),	make_pair(0x5B4B, 0x03),
	make_pair(0x45F9, 0x04),	make_pair(0x40CE, 0x05),
	make_pair(0x4F97, 0x06),	make_pair(0x4AA0, 0x07),
	make_pair(0x77C4, 0x08),	make_pair(0x72F3, 0x09),
	make_pair(0x7DAA, 0x0A),	make_pair(0x789D, 0x0B),
	make_pair(0x662F, 0x0C),	make_pair(0x6318, 0x0D),
	make_pair(0x6C41, 0x0E),	make_pair(0x6976, 0x0F),
	make_pair(0x1689, 0x10),	make_pair(0x13BE, 0x11),
	make_pair(0x1CE7, 0x12),	make_pair(0x19D0, 0x13),
	make_pair(0x0762, 0x14),	make_pair(0x0255, 0x15),
	make_pair(0x0D0C, 0x16),	make_pair(0x083B, 0x17),
	make_pair(0x355F, 0x18),	make_pair(0x3068, 0x19),
	make_pair(0x3F31, 0x1A),	make_pair(0x3A06, 0x1B),
	make_pair(0x24B4, 0x1C),	make_pair(0x2183, 0x1D),
	make_pair(0x2EDA, 0x1E),	make_pair(0x2BED, 0x1F),
};

const map<uint32_t, uint32_t> QrFormatInformation::ENCODED_FORMATS(
		QrFormatInformation_formats_mapping,
		QrFormatInformation_formats_mapping + sizeof QrFormatInformation_formats_mapping
    / sizeof QrFormatInformation_formats_mapping[0]);

void QrFormatInformation::buildXORDataMask(BitMatrix &mask, QrVersionInformation version) {
	Size size = version.getQrBarcodeSize();
	BitMatrix dataMask;
	version.getDataMask(dataMask);

	switch (xorDataMask) {
	case XOR_DATA_MASK_000:
		_buildXORDataMask<XORDataMaskCalc000>(mask, size);
		break;
	case XOR_DATA_MASK_001:
		_buildXORDataMask<XORDataMaskCalc001>(mask, size);
		break;
	case XOR_DATA_MASK_010:
		_buildXORDataMask<XORDataMaskCalc010>(mask, size);
		break;
	case XOR_DATA_MASK_011:
		_buildXORDataMask<XORDataMaskCalc011>(mask, size);
		break;
	case XOR_DATA_MASK_100:
		_buildXORDataMask<XORDataMaskCalc100>(mask, size);
		break;
	case XOR_DATA_MASK_101:
		_buildXORDataMask<XORDataMaskCalc101>(mask, size);
		break;
	case XOR_DATA_MASK_110:
		_buildXORDataMask<XORDataMaskCalc110>(mask, size);
		break;
	case XOR_DATA_MASK_111:
		_buildXORDataMask<XORDataMaskCalc111>(mask, size);
		break;
	}

	mask.maskAND(dataMask);
}

QrFormatInformation QrFormatInformation::fromBitMatrix(const BitMatrix &code, QrVersionInformation version) {
	vector<Rect> formatPositions;
	GridSampler sampler(Size(1, 1), GridSampler::BOTTOM_LEFT, GridSampler::BOTTOM_LEFT, false, false);
	BitArray _result, result;
	uint32_t encodedVersion;
	BitMatrix codeROI;

	version.getFormatPosition1(formatPositions);

	codeROI = code(formatPositions[0]);
	sampler.sample(codeROI, _result);	result.push(_result);
	codeROI = code(formatPositions[1]);
	sampler.sample(codeROI, _result);	result.push(_result);
	codeROI = code(formatPositions[2]);
	sampler.sample(codeROI, _result);	result.push(_result);
	codeROI = code(formatPositions[3]);
	sampler.sample(codeROI, _result);	result.push(_result);

	encodedVersion = result.toULong();

	result.clear();
	version.getFormatPosition2(formatPositions);
	codeROI = code(formatPositions[0]);
	sampler.sample(codeROI, _result);	result.push(_result);
	codeROI = code(formatPositions[1]);
	sampler.sample(codeROI, _result);	result.push(_result);

	encodedVersion = result.toULong();

	return QrFormatInformation(ERROR_CORRECT_LEVEL_H, XOR_DATA_MASK_100);
}


} /* namespace barcodes */

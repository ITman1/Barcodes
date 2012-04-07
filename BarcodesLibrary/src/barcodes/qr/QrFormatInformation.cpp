///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrFormatInformation.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrFormatInformation class which provides
//             informations about QR code of some specific format.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrFormatInformation.cpp
 *
 * @brief Defines members of QrFormatInformation class which provides
 *        informations about QR code of some specific format.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include "../../debug.h"
#include "QrFormatInformation.h"

#define DEBUG_TAG "QrFormatInformation.cpp"

namespace barcodes {

/**
 * Abstract class for functors which builds the xor data masks.
 */
class XORDataMaskCalcFunctor {
public:
	virtual ~XORDataMaskCalcFunctor() {}
	/**
	 * Calculates the mask for specified indexes.
	 *
	 * @param i Row index.
	 * @param j Column index.
	 * @return Mask value.
	 */
	virtual bool calc(int i, int j) = 0;
};

/**
 * Functor for building the 000 data mask.
 */
class XORDataMaskCalc000: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i + j) % 2 == 0;
	}
};

/**
 * Functor for building the 001 data mask.
 */
class XORDataMaskCalc001: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return i % 2 == 0;
	}
};

/**
 * Functor for building the 010 data mask.
 */
class XORDataMaskCalc010: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return j % 3 == 0;
	}
};

/**
 * Functor for building the 011 data mask.
 */
class XORDataMaskCalc011: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i + j) % 3 == 0;
	}
};

/**
 * Functor for building the 100 data mask.
 */
class XORDataMaskCalc100: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i / 2) + (j / 3)) % 2 == 0;
	}
};

/**
 * Functor for building the 101 data mask.
 */
class XORDataMaskCalc101: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return (i * j) % 2 + (i * j) % 3 == 0;
	}
};

/**
 * Functor for building the 110 data mask.
 */
class XORDataMaskCalc110: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i * j) % 2 + (i * j) % 3) % 2 == 0;
	}
};

/**
 * Functor for building the 111 data mask.
 */
class XORDataMaskCalc111: public XORDataMaskCalcFunctor {
public:
	inline bool calc(int i, int j) {
		return ((i + j) % 2 + (i * j) % 3) % 2 == 0;
	}
};

/**
 * Builds XOR data mask where builder functor is passed as a template parameter.
 *
 * @param mask The result built mask.
 * @param version Version for which should be built the mask.
 */
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

/**
 * Constant used for returning of invalid decoded format from the bit matrix.
 */
const QrFormatInformation QrFormatInformation::INVALID_FORMAT(true);

/**
 * Helper array for the creation of the lookup table.
 */
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

/**
 * Lookup table which maps encoded format information to proper decoded.
 */
const LookupTable<uint32_t, uint32_t> QrFormatInformation::ENCODED_FORMATS(
		QrFormatInformation_formats_mapping,
		QrFormatInformation_formats_mapping + sizeof QrFormatInformation_formats_mapping
    / sizeof QrFormatInformation_formats_mapping[0]);

bool QrFormatInformation::operator==(const QrFormatInformation &rhs) const {
  return xorDataMask == rhs.xorDataMask && errorCorrectionLevel == rhs.errorCorrectionLevel && isInvalid == isInvalid;
}

bool QrFormatInformation::operator!=(const QrFormatInformation &rhs) const {
  return !(*this == rhs);
}

/**
 * Returns XOR mask for this format information.
 *
 * @return XOR mask for this format information.
 */
QrFormatInformation::XORDataMask QrFormatInformation::getXORDataMask() const {
	return xorDataMask;
}

/**
 * Returns error correction level for this format information.
 *
 * @return Error correction level for this format information.
 */
QrFormatInformation::ErrorCorrectionLevel QrFormatInformation::getErrorCorrectionLevel() const {
	return errorCorrectionLevel;
}

/**
 * Builds XOR data mask by which is masked QR code after encoding.
 *
 * @param mask The result built mask.
 * @param version Version for which should be built the mask.
 */
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

/**
 * Decodes format information from the bit matrix.
 *
 * @param code Bit matrix of the QR code.
 * @param version Version of the QR code.
 * @return Decoded format information on success else INVALID_FORMAT.
 */
QrFormatInformation QrFormatInformation::fromBitMatrix(const BitMatrix &code, QrVersionInformation version) {

	// See 6.9 Format information (ISO 18004:2006)

	vector<Rect> formatPositions;
	QrFormatInformation formatInformation(false);

	version.getFormatPosition1(formatPositions);
	formatInformation = decodeFormat(code, formatPositions);

	if (formatInformation != INVALID_FORMAT) return formatInformation;

	version.getFormatPosition2(formatPositions);
	formatInformation = decodeFormat(code, formatPositions);

	return formatInformation;
}

/**
 * Decodes format information from the bit matrix for specified format positions.
 *
 * @param code Bit matrix of the QR code.
 * @param formatPositions Positions of the format information in the bit matrix.
 * @return Decoded format information on success else INVALID_FORMAT.
 */
QrFormatInformation QrFormatInformation::decodeFormat(const BitMatrix &code, vector<Rect> &formatPositions) {
	DEBUG_PRINT(DEBUG_TAG, "decodeFormat()");
	GridSampler sampler(Size(1, 1), GridSampler::LEFT_BOTTOM, GridSampler::BOTTOM_LEFT, false, false);
	BitArray _result, result;
	uint32_t encodedFormat;
	BitMatrix codeROI;

	// Sampling format information
	for (vector<Rect>::iterator formatIter = formatPositions.begin(); formatIter != formatPositions.end(); formatIter++) {
		codeROI = code(*formatIter);
		sampler.sample(codeROI, _result);	result.push(_result);
	}
	encodedFormat = result.toULong();

	// Looking at the version in the look up table and repairing the version if possible
    if ((ENCODED_FORMATS.find(encodedFormat) != ENCODED_FORMATS.end()) ||
    		(ENCODED_FORMATS.correctEncoded(encodedFormat, ENCODED_FORMAT_MAX_CORRECTIONS))) {
		DEBUG_PRINT(DEBUG_TAG, "Format decode success!");
		uint32_t format = ENCODED_FORMATS.at(encodedFormat);

		// Decoded format: 3 bits of the data mask and 2 bits of the error correction level
		ErrorCorrectionLevel errCorrection = ErrorCorrectionLevel((format & 0x18) >> 3);
		XORDataMask xorMask = XORDataMask(format & 0x07);

    	return QrFormatInformation(errCorrection, xorMask);
    } else { // string for this capability is not defined
		DEBUG_PRINT(DEBUG_TAG, "Format decode failed!");
        return INVALID_FORMAT;
    }

}


} /* namespace barcodes */

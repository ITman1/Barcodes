///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrFormatInformation.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrFormatInformation class which provides informations
//             about QR code of some specific format.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrFormatInformation.h
 *
 * @brief Defines QrFormatInformation class which provides informations
 *        about QR code of some specific format.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRFORMATINFORMATION_H_
#define QRFORMATINFORMATION_H_

#include <opencv2/core/core.hpp>

#include "../common/errcontrol/LookupTable.h"
#include "../DetectedMarks.h"
#include "../common/GridSampler.h"
#include "QrVersionInformation.h"

namespace barcodes {
using namespace cv;

/**
 * Class QrFormatInformation serves mostly for decoding the information about
 * format from the bit matrix and keeping this decoded informations.
 */
class QrFormatInformation {
public:

	/**
	 * Enumeration of possible correction levels.
	 */
	typedef enum error_correction_levels {
		ERROR_CORRECT_LEVEL_L = 0x1,//!< ERROR_CORRECT_LEVEL_L
		ERROR_CORRECT_LEVEL_M = 0x0,//!< ERROR_CORRECT_LEVEL_M
		ERROR_CORRECT_LEVEL_Q = 0x3,//!< ERROR_CORRECT_LEVEL_Q
		ERROR_CORRECT_LEVEL_H = 0x2 //!< ERROR_CORRECT_LEVEL_H
	} ErrorCorrectionLevel;

	/**
	 * Enumeration of possible XOR masks used for data.
	 */
	typedef enum xor_data_masks {
		XOR_DATA_MASK_000 = 0x0,//!< XOR_DATA_MASK_000
		XOR_DATA_MASK_001 = 0x1,//!< XOR_DATA_MASK_001
		XOR_DATA_MASK_010 = 0x2,//!< XOR_DATA_MASK_010
		XOR_DATA_MASK_011 = 0x3,//!< XOR_DATA_MASK_011
		XOR_DATA_MASK_100 = 0x4,//!< XOR_DATA_MASK_100
		XOR_DATA_MASK_101 = 0x5,//!< XOR_DATA_MASK_101
		XOR_DATA_MASK_110 = 0x6,//!< XOR_DATA_MASK_110
		XOR_DATA_MASK_111 = 0x7 //!< XOR_DATA_MASK_111
	} XORDataMask;

	/**
	 * Constant used for returning of invalid decoded format from the bit matrix.
	 */
	const static QrFormatInformation INVALID_FORMAT;

	/**
	 * Creates a new version information.
	 *
	 * @param errorCorrectionLevel Error correction level.
	 * @param xorDataMask XOR data mask.
	 */
	QrFormatInformation(ErrorCorrectionLevel errorCorrectionLevel, XORDataMask xorDataMask) :
		errorCorrectionLevel(errorCorrectionLevel), xorDataMask(xorDataMask), isInvalid(false) {}
	virtual ~QrFormatInformation() {}

	/**
	 * Builds XOR data mask by which is masked QR code after encoding.
	 *
	 * @param mask The result built mask.
	 * @param version Version for which should be built the mask.
	 */
	void buildXORDataMask(BitMatrix &mask, QrVersionInformation version);

	bool operator!=(const QrFormatInformation &rhs) const;
	bool operator==(const QrFormatInformation &rhs) const;

	/**
	 * Returns XOR mask for this format information.
	 *
	 * @return XOR mask for this format information.
	 */
	XORDataMask getXORDataMask() const;

	/**
	 * Returns error correction level for this format information.
	 *
	 * @return Error correction level for this format information.
	 */
	ErrorCorrectionLevel getErrorCorrectionLevel() const;

	/**
	 * Decodes format information from the bit matrix.
	 *
	 * @param code Bit matrix of the QR code.
	 * @param version Version of the QR code.
	 * @return Decoded format information on success else INVALID_FORMAT.
	 */
	static QrFormatInformation fromBitMatrix(const BitMatrix &code, QrVersionInformation version);
private:
	/**
	 * Lookup table which maps encoded format information to proper decoded.
	 */
	const static LookupTable<uint32_t, uint32_t> ENCODED_FORMATS;

	/**
	 * Maximal number of correction which are allowed for encoded version information in the image.
	 */
	const static int ENCODED_FORMAT_MAX_CORRECTIONS = 3;

	/**
	 * Error correction level.
	 */
	ErrorCorrectionLevel errorCorrectionLevel;

	/**
	 * XOR data mask.
	 */
	XORDataMask xorDataMask;

	/**
	 * Signals that this format is invalid. Is set only in the constant INVALID_FORMAT.
	 */
	bool isInvalid;

	/**
	 * Constructs especially invalid format constant.
	 *
	 * @param isInvalid Signals whether is invalid format is invalid.
	 */
	QrFormatInformation(bool isInvalid) : errorCorrectionLevel(ERROR_CORRECT_LEVEL_L),
		xorDataMask(XOR_DATA_MASK_000), isInvalid(isInvalid) {}

	/**
	 * Decodes format information from the bit matrix for specified format positions.
	 *
	 * @param code Bit matrix of the QR code.
	 * @param formatPositions Positions of the format information in the bit matrix.
	 * @return Decoded format information on success else INVALID_FORMAT.
	 */
	static QrFormatInformation decodeFormat(const BitMatrix &code, vector<Rect> &formatPositions);
};

} /* namespace barcodes */
#endif /* QRFORMATINFORMATION_H_ */

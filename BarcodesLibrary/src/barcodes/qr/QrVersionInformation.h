///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrVersionInformation.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrVersionInformation class which provides informations
//             about QR code of some specific version.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrVersionInformation.h
 *
 * @brief Defines QrVersionInformation class which provides informations
 *        about QR code of some specific version.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRVERSIONINFORMATION_H_
#define QRVERSIONINFORMATION_H_

#include <map>
#include <opencv2/core/core.hpp>

#include "../common/errcontrol/LookupTable.h"
#include "../DetectedMarks.h"
#include "../common/GridSampler.h"

namespace barcodes {

#define QR_SIZE(version) 17 + 4 * version

#define _COMMA ,
#define _SEMICOLON ;

// Declares version class
#define _CLASSES(num,nothing) static const QrVersionInformation VERSION_ ## num

// Defines version class
#define _CLASSES_DEFINE(num,name_space) \
		const QrVersionInformation name_space ::VERSION_ ## num(num)

// Generation macro for all versions
// M - name of macro which generates content
// D - delimiter by which is generated content separated
// N - some additional information passed into generation macro eg. namespace
#define _VERSIONS_MACRO(M,D,N) \
	M(1,N)D M(2,N)D M(3,N)D M(4,N)D M(5,N)D M(6,N)D M(7,N)D M(8,N)D M(9,N)D M(10,N)D \
	M(11,N)D M(12,N)D M(13,N)D M(14,N)D M(15,N)D M(16,N)D M(17,N)D M(18,N)D M(19,N)D M(20,N)D \
	M(21,N)D M(22,N)D M(23,N)D M(24,N)D M(25,N)D M(26,N)D M(27,N)D M(28,N)D M(29,N)D M(30,N)D \
	M(31,N)D M(32,N)D M(33,N)D M(34,N)D M(35,N)D M(36,N)D M(37,N)D M(38,N)D M(39,N)D M(40,N)

// Declares version classes
#define DECLARE_VERSION_CLASSES _VERSIONS_MACRO(_CLASSES,_SEMICOLON,) _SEMICOLON

// Defines version classes
#define DEFINE_VERSION_CLASSES(name_space) _VERSIONS_MACRO(_CLASSES_DEFINE,_SEMICOLON,QrVersionInformation) _SEMICOLON

using namespace cv;
using namespace std;

/**
 * Class QrVersionInformation gathers some methods which depend on the
 * version of QR code such as positions of marks/patterns/codeword size etc.
 */
class QrVersionInformation {
private:
	/**
	 * Version of the QR code.
	 */
	int version;
	/**
	 * Size of the first information about version in the QR code.
	 */
	const static Size VERSION_POSITION1_SIZE;

	/**
	 * Size of the second information about version in the QR code.
	 */
	const static Size VERSION_POSITION2_SIZE;

	/**
	 * Lookup table which maps encoded version information to proper version.
	 */
	const static LookupTable<uint32_t, uint32_t> ENCODED_VERSIONS;

	/**
	 * Maximal number of all possible center coordinates inside ALIGNMENT_PATTERNS_LOOKUP_TABLE.
	 */
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS = 7;

	/**
	 * Number of QR code versions.
	 */
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS = 40;

	/**
	 * Maps version number to coordinates of the centers of alignment marks.
	 */
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE[ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS][ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS];

	/**
	 * Maximal number of correction which are allowed for encoded version information in the image.
	 */
	const static int ENCODED_VERSION_MAX_CORRECTIONS = 3;

	/**
	 * Decodes version from the bit matrix and returns decoded version.
	 *
	 * @param bitMatrix Bit matrix of the QR code.
	 * @param bitsDirection Direction of the sampling of version information.
	 * @return Decoded information version on success, else INVALID_VERSION.
	 */
	static QrVersionInformation decodeVersion(BitMatrix &bitMatrix, GridSampler::FlowDirection bitsDirection);
public:
	DECLARE_VERSION_CLASSES
	/**
	 * Constant which is used for returning invalid versions.
	 */
	const static QrVersionInformation INVALID_VERSION;

	/**
	 * Constant which is used for returning invalid positions.
	 */
	const static Rect INVALID_POSITION;

	/**
	 * Constant which is used for returning invalid sizes.
	 */
	const static Size INVALID_VERSION_SIZE;

	/**
	 * Constructs version information for specified version.
	 *
	 * @param version Version of the QR code.
	 */
	QrVersionInformation(int version);
	virtual ~QrVersionInformation() {}

	/**
	 * Returns version of the QR code.
	 *
	 * @return Version of the QR code.
	 */
	int getVersion() const;

	/**
	 * Returns count of the codewords which are present inside QR code for this version.
	 *
	 * @return Count of the codewords which are present inside QR code for this version.
	 */
	int getCodewordsCount() const;

	/**
	 * Returns size (number of bits) of codeword of this version.
	 *
	 * @return Size (number of bits) of codeword of this version.
	 */
	int getCodewordSize() const;

	/**
	 * Returns position of the first version information inside bit matrix.
	 *
	 * @return Position of the first version information inside bit matrix.
	 */
	Rect getVersionPosition1() const;

	/**
	 * Returns position of the second version information inside bit matrix.
	 *
	 * @return Position of the second version information inside bit matrix.
	 */
	Rect getVersionPosition2() const;

	/**
	 * Returns position of the first timer pattern inside bit matrix.
	 *
	 * @return Position of the first timer pattern inside bit matrix.
	 */
	Rect getTimerPattern1Position() const;

	/**
	 * Returns position of the second timer pattern inside bit matrix.
	 *
	 * @return Position of the second timer pattern inside bit matrix.
	 */
	Rect getTimerPattern2Position() const;

	/**
	 * Returns positions of the first format information inside bit matrix.
	 *
	 * @param formatPositions Positions of the first format information inside bit matrix.
	 */
	void getFormatPosition1(vector<Rect> &formatPositions) const;

	/**
	 * Returns positions of the second format information inside bit matrix.
	 *
	 * @param formatPositions Positions of the second format information inside bit matrix.
	 */
	void getFormatPosition2(vector<Rect> &formatPositions) const;

	/**
	 * Returns positions of the finder patterns inside bit matrix.
	 *
	 * @param finderPatterns Positions of the finder patterns inside bit matrix.
	 */
	void getFinderPatternPositions(vector<Rect> &finderPatterns) const;

	/**
	 * Returns positions of the alignment patterns inside bit matrix.
	 *
	 * @param alignmentPatterns Positions of the alignment patterns inside bit matrix.
	 */
	void getAlignmentPatternPositions(vector<Rect> &alignmentPatterns) const;

	/**
	 * Returns positions of the other modules which should be masked or filled somehow.
	 *
	 * @param maskPositions Positions of the other modules which should be masked or filled somehow.
	 */
	void getOtherMaskPositions(vector<Rect> &maskPositions) const;

	/**
	 * Returns mask which masks all patterns from the bit matrix.
	 * It is used for reading the data from QR code.
	 *
	 * @param mask Result mask for masking the QR code and revealing the data.
	 */
	void getDataMask(BitMatrix &mask) const;

	/**
	 * Returns the size of the bit matrix demanded for QR code of this version.
	 *
	 * @return Size of the bit matrix demanded for QR code of this version.
	 */
	Size getQrBarcodeSize() const;

	bool operator!=(const QrVersionInformation &rhs) const;
	bool operator==(const QrVersionInformation &rhs) const;
	bool operator>(const QrVersionInformation &rhs) const;
	bool operator<(const QrVersionInformation &rhs) const;
	bool operator>=(const QrVersionInformation &rhs) const;
	bool operator<=(const QrVersionInformation &rhs) const;

	/**
	 * Decodes version from the image and returns decoded version.
	 *
	 * @param image Image which contains QR code.
	 * @param sortedDetectedMarks Localization marks of the QR code, points are sorted as defined in the QrDecoder class.
	 * @return Decoded information version on success, else INVALID_VERSION.
	 */
	static QrVersionInformation fromImage(const Mat &image, const DetectedMarks &sortedDetectedMarks);
};

} /* namespace barcodes */
#endif /* QRVERSIONINFORMATION_H_ */

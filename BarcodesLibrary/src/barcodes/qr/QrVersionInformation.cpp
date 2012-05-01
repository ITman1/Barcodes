///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrVersionInformation.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the QrVersionInformation class which provides
//             informations about QR code of some specific version.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrVersionInformation.cpp
 *
 * @brief Defines members of the QrVersionInformation class which provides
 *        informations about QR code of some specific version.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>

#include "QrVersionInformation.h"
#include "../../debug.h"
#include "../../common/Vector2D.h"
#include "../../common/Line2D.h"

#define DEBUG_TAG "QrVersionInformation.cpp"

namespace barcodes {

DEFINE_VERSION_CLASSES(QrVersionInformation);

/**
 * Constant which is used for returning invalid versions.
 */
const QrVersionInformation QrVersionInformation::INVALID_VERSION(-1);

/**
 * Constant which is used for returning invalid positions.
 */
const Rect QrVersionInformation::INVALID_POSITION(0, 0, 0, 0);

/**
 * Constant which is used for returning invalid sizes.
 */
const Size QrVersionInformation::INVALID_VERSION_SIZE(-1, -1);

/**
 * Size of the first information about version in the QR code.
 */
const Size QrVersionInformation::VERSION_POSITION1_SIZE(3, 6);

/**
 * Size of the second information about version in the QR code.
 */
const Size QrVersionInformation::VERSION_POSITION2_SIZE(6, 3);

/**
 * Helper array for the creation of the lookup table.
 */
pair<uint32_t, uint32_t> QrVersionInformation_versions_mapping[] = {
    make_pair(0x07C94,  7),    make_pair(0x085BC,  8),
    make_pair(0x09A99,  9),    make_pair(0x0A4D3, 10),
    make_pair(0x0BBF6, 11),    make_pair(0x0C762, 12),
    make_pair(0x0D847, 13),    make_pair(0x0E60D, 14),
    make_pair(0x0F928, 15),    make_pair(0x10B78, 16),
    make_pair(0x1145D, 17),    make_pair(0x12A17, 18),
    make_pair(0x13532, 19),    make_pair(0x149A6, 20),
    make_pair(0x15683, 21),    make_pair(0x168C9, 22),
    make_pair(0x177EC, 23),    make_pair(0x18EC4, 24),
    make_pair(0x191E1, 25),    make_pair(0x1AFAB, 26),
    make_pair(0x1B08E, 27),    make_pair(0x1CC1A, 28),
    make_pair(0x1D33F, 29),    make_pair(0x1ED75, 30),
    make_pair(0x1F250, 31),    make_pair(0x209D5, 32),
    make_pair(0x216F0, 33),    make_pair(0x228BA, 34),
    make_pair(0x2379F, 35),    make_pair(0x24B0B, 36),
    make_pair(0x2542E, 37),    make_pair(0x26A64, 38),
    make_pair(0x27541, 39),    make_pair(0x28C69, 40)
};

/**
 * Lookup table which maps encoded version information to proper version.
 */
const LookupTable<uint32_t, uint32_t> QrVersionInformation::ENCODED_VERSIONS(
		QrVersionInformation_versions_mapping,
    QrVersionInformation_versions_mapping + sizeof QrVersionInformation_versions_mapping
    / sizeof QrVersionInformation_versions_mapping[0]);

/**
 * Maps version number to coordinates of the centers of alignment marks.
 */
const int QrVersionInformation::ALIGNMENT_PATTERNS_LOOKUP_TABLE[QrVersionInformation::
    ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS][QrVersionInformation::ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS] = {
	{-1, -1, -1, -1,  -1,  -1,  -1},	{ 6, 18, -1, -1,  -1,  -1,  -1},
	{ 6, 22, -1, -1,  -1,  -1,  -1},	{ 6, 26, -1, -1,  -1,  -1,  -1},
	{ 6, 30, -1, -1,  -1,  -1,  -1},	{ 6, 34, -1, -1,  -1,  -1,  -1},
	{ 6, 22, 38, -1,  -1,  -1,  -1},	{ 6, 24, 42, -1,  -1,  -1,  -1},
	{ 6, 26, 46, -1,  -1,  -1,  -1},	{ 6, 28, 50, -1,  -1,  -1,  -1},
	{ 6, 30, 54, -1,  -1,  -1,  -1},	{ 6, 32, 58, -1,  -1,  -1,  -1},
	{ 6, 34, 62, -1,  -1,  -1,  -1},	{ 6, 26, 46, 66,  -1,  -1,  -1},
	{ 6, 26, 48, 70,  -1,  -1,  -1},	{ 6, 26, 50, 74,  -1,  -1,  -1},
	{ 6, 30, 54, 78,  -1,  -1,  -1},	{ 6, 30, 56, 82,  -1,  -1,  -1},
	{ 6, 30, 58, 86,  -1,  -1,  -1},	{ 6, 34, 62, 90,  -1,  -1,  -1},
	{ 6, 28, 50, 72,  94,  -1,  -1},	{ 6, 26, 50, 74,  98,  -1,  -1},
	{ 6, 30, 54, 78, 102,  -1,  -1},	{ 6, 28, 54, 80, 106,  -1,  -1},
	{ 6, 32, 58, 84, 110,  -1,  -1},	{ 6, 30, 58, 86, 114,  -1,  -1},
	{ 6, 34, 62, 90, 118,  -1,  -1},	{ 6, 26, 50, 74,  98, 122,  -1},
	{ 6, 30, 54, 78, 102, 126,  -1},	{ 6, 26, 52, 78, 104, 130,  -1},
	{ 6, 30, 56, 82, 108, 134,  -1},	{ 6, 34, 60, 86, 112, 138,  -1},
	{ 6, 30, 58, 86, 114, 142,  -1},	{ 6, 34, 62, 90, 118, 146,  -1},
	{ 6, 30, 54, 78, 102, 126, 150},	{ 6, 24, 50, 76, 102, 128, 154},
	{ 6, 28, 54, 80, 106, 132, 158},	{ 6, 32, 58, 84, 110, 136, 162},
	{ 6, 26, 54, 82, 110, 138, 166},    { 6, 30, 58, 86, 114, 142, 170}
};

/**
 * Constructs version information for specified version.
 *
 * @param version Version of the QR code.
 */
QrVersionInformation::QrVersionInformation(int version) : version(version) {
	if ((version < VERSION_1.getVersion()) || (version > VERSION_40.getVersion())) {
		version = -1;
	}
}

bool QrVersionInformation::operator==(const QrVersionInformation &rhs) const {
	return version == rhs.version;
}

bool QrVersionInformation::operator!=(const QrVersionInformation &rhs) const {
	return version != rhs.version;
}

bool QrVersionInformation::operator>(const QrVersionInformation &rhs) const {
	  return version > rhs.version;
}

bool QrVersionInformation::operator<(const QrVersionInformation &rhs) const {
	  return version < rhs.version;
}

bool QrVersionInformation::operator>=(const QrVersionInformation &rhs) const {
	  return version >= rhs.version;
}

bool QrVersionInformation::operator<=(const QrVersionInformation &rhs) const {
	  return version <= rhs.version;
}

/**
 * Returns position of the first version information inside bit matrix.
 *
 * @return Position of the first version information inside bit matrix.
 */
Rect QrVersionInformation::getVersionPosition1() const {
	if (version <= VERSION_6.getVersion()) {
		return INVALID_POSITION;
	} else {
		Point startPoint(QR_SIZE(version) - 11, 0);
		return Rect(startPoint, VERSION_POSITION1_SIZE);
	}
}

/**
 * Returns position of the second version information inside bit matrix.
 *
 * @return Position of the second version information inside bit matrix.
 */
Rect QrVersionInformation::getVersionPosition2() const {
	if (version <= VERSION_6.getVersion()) {
		return INVALID_POSITION;
	} else {
		Point startPoint(0, QR_SIZE(version) - 11);
		return Rect(startPoint, VERSION_POSITION2_SIZE);
	}
}

/**
 * Returns the size of the bit matrix demanded for QR code of this version.
 *
 * @return Size of the bit matrix demanded for QR code of this version.
 */
Size QrVersionInformation::getQrBarcodeSize() const {
	return (version < VERSION_1.getVersion())?
			INVALID_VERSION_SIZE : Size(QR_SIZE(version), QR_SIZE(version));
}

/**
 * Returns positions of the first format information inside bit matrix.
 *
 * @param formatPositions Positions of the first format information inside bit matrix.
 */
void QrVersionInformation::getFormatPosition1(vector<Rect> &formatPositions) const {
	formatPositions.clear();
	formatPositions.push_back(Rect(Point(8, 0), Size(1, 6)));
	formatPositions.push_back(Rect(Point(8, 7), Size(1, 2)));
	formatPositions.push_back(Rect(Point(7, 8), Size(1, 1)));
	formatPositions.push_back(Rect(Point(0, 8), Size(6, 1)));
}

/**
 * Returns positions of the second format information inside bit matrix.
 *
 * @param formatPositions Positions of the second format information inside bit matrix.
 */
void QrVersionInformation::getFormatPosition2(vector<Rect> &formatPositions) const {
	formatPositions.clear();
	formatPositions.push_back(Rect(Point(QR_SIZE(version) - 8, 8), Size(8, 1)));
	formatPositions.push_back(Rect(Point(8, QR_SIZE(version) - 7), Size(1, 7)));
}

/**
 * Returns positions of the finder patterns inside bit matrix.
 *
 * @param finderPatterns Positions of the finder patterns inside bit matrix.
 */
void QrVersionInformation::getFinderPatternPositions(vector<Rect> &finderPatterns) const {
	finderPatterns.clear();
	finderPatterns.push_back(Rect(Point(0, 0), Size(7,7)));
	finderPatterns.push_back(Rect(Point(QR_SIZE(version) - 7, 0), Size(7,7)));
	finderPatterns.push_back(Rect(Point(0, QR_SIZE(version) - 7), Size(7,7)));
}

/**
 * Returns positions of the alignment patterns inside bit matrix.
 *
 * @param alignmentPatterns Positions of the alignment patterns inside bit matrix.
 */
void QrVersionInformation::getAlignmentPatternPositions(vector<Rect> &alignmentPatterns) const {
	alignmentPatterns.clear();

	const int *center = ALIGNMENT_PATTERNS_LOOKUP_TABLE[version - 1];
	if (*center == -1) return;

	vector<int> centers;
	for (int i = 0; i < ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS; i++) {
		if (*center != -1) {
			centers.push_back(*center);
		} else {
			break;
		}
		center++;
	}

	vector<int>::iterator iter1, iter2;
	Point centerPoint;
	Point filterPoint1 = Point(centers.front(), centers.front());
	Point filterPoint2 = Point(centers.front(), centers.back());
	Point filterPoint3 = Point(centers.back(), centers.front());
	for (iter1 = centers.begin(); iter1 != centers.end(); iter1++) {
		for (iter2 = centers.begin(); iter2 != centers.end(); iter2++) {
			centerPoint = Point(*iter1, *iter2);
			if (centerPoint != filterPoint1 && centerPoint != filterPoint2 && centerPoint != filterPoint3) {
				alignmentPatterns.push_back(Rect(centerPoint - Point(2, 2), Size(5, 5)));
			}
		}
	}

}

/**
 * Returns position of the first timer pattern inside bit matrix.
 *
 * @return Position of the first timer pattern inside bit matrix.
 */
Rect QrVersionInformation::getTimerPattern1Position() const {
	return Rect(Point(8, 6), Size(QR_SIZE(version) - 16, 1));
}

/**
 * Returns position of the second timer pattern inside bit matrix.
 *
 * @return Position of the second timer pattern inside bit matrix.
 */
Rect QrVersionInformation::getTimerPattern2Position() const {
	return Rect(Point(6, 8), Size(1, QR_SIZE(version) - 16));
}

/**
 * Returns positions of the other modules which should be masked or filled somehow.
 *
 * @param maskPositions Positions of the other modules which should be masked or filled somehow.
 */
void QrVersionInformation::getOtherMaskPositions(vector<Rect> &maskPositions) const {
	maskPositions.clear();
	maskPositions.push_back(Rect(Point(8, QR_SIZE(version) - 8), Size(1, 1)));

	maskPositions.push_back(Rect(Point(7, 0), Size(1, 8)));
	maskPositions.push_back(Rect(Point(0, 7), Size(7, 1)));

	maskPositions.push_back(Rect(Point(QR_SIZE(version) - 8, 0), Size(1, 8)));
	maskPositions.push_back(Rect(Point(QR_SIZE(version) - 7, 7), Size(7, 1)));

	maskPositions.push_back(Rect(Point(0, QR_SIZE(version) - 8), Size(8, 1)));
	maskPositions.push_back(Rect(Point(7, QR_SIZE(version) - 7), Size(1, 7)));
}

/**
 * Returns mask which masks all patterns from the bit matrix.
 * It is used for reading the data from QR code.
 *
 * @param mask Result mask for masking the QR code and revealing the data.
 */
void QrVersionInformation::getDataMask(BitMatrix &mask) const {
	mask = BitMatrix(getQrBarcodeSize(), true);

	vector<Rect> maskRects;
	maskRects.push_back(getVersionPosition1());
	maskRects.push_back(getVersionPosition2());
	maskRects.push_back(getTimerPattern1Position());
	maskRects.push_back(getTimerPattern2Position());
	mask.fillRects(maskRects, false);

	getFormatPosition1(maskRects);
	mask.fillRects(maskRects, false);

	getFormatPosition2(maskRects);
	mask.fillRects(maskRects, false);

	getFinderPatternPositions(maskRects);
	mask.fillRects(maskRects, false);

	getAlignmentPatternPositions(maskRects);
	mask.fillRects(maskRects, false);

	getOtherMaskPositions(maskRects);
	mask.fillRects(maskRects, false);
}

/**
 * Decodes version from the image and returns decoded version.
 *
 * @param image Image which contains QR code.
 * @param sortedDetectedMarks Localization marks of the QR code, points are sorted as defined in the QrDecoder class.
 * @return Decoded information version on success, else INVALID_VERSION.
 */
QrVersionInformation QrVersionInformation::fromImage(const Mat &image, const DetectedMarks &sortedDetectedMarks) {

	// See steps 1-7 in 11 Reference decode algorithm for QR Code 2005 (ISO 18004:2006)
	// and for sample direction 6.10 Version information (ISO 18004:2006)

	double W_UL = (Vector2D(sortedDetectedMarks[1].points[0], sortedDetectedMarks[1].points[1]).size() +
			Vector2D(sortedDetectedMarks[1].points[2], sortedDetectedMarks[1].points[3]).size()) / 2;
	double W_UR = (Vector2D(sortedDetectedMarks[2].points[3], sortedDetectedMarks[2].points[0]).size() +
			Vector2D(sortedDetectedMarks[2].points[1], sortedDetectedMarks[2].points[2]).size()) / 2;
	Point C_UL, C_UR;
	Line2D::intersection(Line2D(sortedDetectedMarks[1].points[0], sortedDetectedMarks[1].points[2]),
			Line2D(sortedDetectedMarks[1].points[1], sortedDetectedMarks[1].points[3]), C_UL);
	Line2D::intersection(Line2D(sortedDetectedMarks[2].points[0], sortedDetectedMarks[2].points[2]),
			Line2D(sortedDetectedMarks[2].points[1], sortedDetectedMarks[2].points[3]), C_UR);
	int D = Vector2D(C_UL, C_UR).size();
	double X = (W_UL + W_UR) / 14.0;
	int V = round(((D / (double)X) - 10) / 4.0);

	if (V < VERSION_1.getVersion()) return INVALID_VERSION;
	if (V <= VERSION_6.getVersion()) return QrVersionInformation(V);

	BitMatrix versionBitMatrix;

	// VERSION NEAR THE UPPER RIGHT MARK
	double CP_UR = W_UR / (double) 7;
	BitMatrix::fromImage(image, VERSION_POSITION1_SIZE, versionBitMatrix, Rect(image.cols - 11 * CP_UR, 0,
			VERSION_POSITION1_SIZE.width * CP_UR, VERSION_POSITION1_SIZE.height * CP_UR));

	QrVersionInformation version = decodeVersion(versionBitMatrix, GridSampler::RIGHT_BOTTOM);
	if (version != INVALID_VERSION) return version;

	// VERSION NEAR THE DOWN LEFT MARK
	double W_DL = (Vector2D(sortedDetectedMarks[0].points[0], sortedDetectedMarks[0].points[3]).size() +
			Vector2D(sortedDetectedMarks[0].points[1], sortedDetectedMarks[0].points[2]).size()) / 2;
	double CP_DL = W_DL / (double) 7;

	BitMatrix::fromImage(image, VERSION_POSITION2_SIZE, versionBitMatrix, Rect(0, image.cols - 11 * CP_DL,
			VERSION_POSITION2_SIZE.width * CP_DL, VERSION_POSITION2_SIZE.height * CP_DL));

	version = decodeVersion(versionBitMatrix, GridSampler::BOTTOM_RIGHT);
	if (version != INVALID_VERSION) return version;

#ifdef TARGET_DEBUG
	return INVALID_VERSION;
#else
	return ((V > VERSION_40.getVersion()) || (V < VERSION_1.getVersion()))? INVALID_VERSION : QrVersionInformation(V);
#endif
}

/**
 * Decodes version from the bit matrix and returns decoded version.
 *
 * @param bitMatrix Bit matrix of the QR code.
 * @param bitsDirection Direction of the sampling of version information.
 * @return Decoded information version on success, else INVALID_VERSION.
 */
QrVersionInformation QrVersionInformation::decodeVersion(BitMatrix &bitMatrix, GridSampler::FlowDirection bitsDirection) {
	DEBUG_PRINT(DEBUG_TAG, "decodeVersion()");
	GridSampler sampler(Size(bitMatrix.cols, bitMatrix.rows), bitsDirection, bitsDirection, false, false);
	BitArray result;

	// Sampling the bit matrix
	sampler.sample(bitMatrix, result);
	uint32_t encodedVersion = result.toNumber<uint32_t>();

	// Looking at the version in the look up table and repairing the version if possible
    if ((ENCODED_VERSIONS.find(encodedVersion) != ENCODED_VERSIONS.end()) ||
    		(ENCODED_VERSIONS.correctEncoded(encodedVersion, ENCODED_VERSION_MAX_CORRECTIONS))) {
		DEBUG_PRINT(DEBUG_TAG, "Version decode success!");
    	return QrVersionInformation(ENCODED_VERSIONS.at(encodedVersion));
    } else {
		DEBUG_PRINT(DEBUG_TAG, "Version decode failed!");
        return INVALID_VERSION;
    }

}

/**
 * Returns version of the QR code.
 *
 * @return Version of the QR code.
 */
int QrVersionInformation::getVersion() const {
	return version;
}

/**
 * Returns count of the codewords which are present inside QR code for this version.
 *
 * @return Count of the codewords which are present inside QR code for this version.
 */
int QrVersionInformation::getCodewordsCount() const {
	BitMatrix mask;
	getDataMask(mask);
	return countNonZero(mask) / getCodewordSize();
}

/**
 * Returns size (number of bits) of codeword of this version.
 *
 * @return Size (number of bits) of codeword of this version.
 */
int QrVersionInformation::getCodewordSize() const {
	return 8;
}

} /* namespace barcodes */

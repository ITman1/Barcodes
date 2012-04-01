/*
 * QrVersionInformation.h
 *
 *  Created on: 25.3.2012
 *      Author: Scotty
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

#define _NUMBERS(num,name_space) VERSION_ ## num = num
#define _VERSIONS(num,name_space) static const QrVersionInformation(num)
#define _CLASSES(num,name_space) static const QrVersionInformation VERSION_ ## num

#define _CLASSES_DEFINE(num,name_space) \
		const QrVersionInformation name_space ::VERSION_ ## num(num)

#define _VERSIONS_MACRO(M,D,N) \
	M(1,N)D M(2,N)D M(3,N)D M(4,N)D M(5,N)D M(6,N)D M(7,N)D M(8,N)D M(9,N)D M(10,N)D \
	M(11,N)D M(12,N)D M(13,N)D M(14,N)D M(15,N)D M(16,N)D M(17,N)D M(18,N)D M(19,N)D M(20,N)D \
	M(21,N)D M(22,N)D M(23,N)D M(24,N)D M(25,N)D M(26,N)D M(27,N)D M(28,N)D M(29,N)D M(30,N)D \
	M(31,N)D M(32,N)D M(33,N)D M(34,N)D M(35,N)D M(36,N)D M(37,N)D M(38,N)D M(39,N)D M(40,N)

#define ENUM_VERSION_NUMBERS _VERSIONS_MACRO(_NUMBERS,_COMMA,)
#define DECLARE_VERSION_CLASSES _VERSIONS_MACRO(_CLASSES,_SEMICOLON,) _SEMICOLON
#define DEFINE_VERSION_CLASSES(name_space) _VERSIONS_MACRO(_CLASSES_DEFINE,_SEMICOLON,QrVersionInformation) _SEMICOLON

using namespace cv;
using namespace std;

class QrVersionInformation {
private:
	int version;
	const static Size VERSION_POSITION1_SIZE;
	const static Size VERSION_POSITION2_SIZE;
	const static LookupTable<uint32_t, uint32_t> ENCODED_VERSIONS;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS = 7;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS = 40;
	const static int ENCODED_VERSION_MAX_CORRECTIONS = 3;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE[ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS][ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS];

	static QrVersionInformation decodeVersion(BitMatrix &bitMatrix, GridSampler::FlowDirection bitsDirection);
public:
	DECLARE_VERSION_CLASSES
	const static QrVersionInformation INVALID_VERSION;
	const static Rect INVALID_POSITION;
	const static Size INVALID_VERSION_SIZE;

	QrVersionInformation(int version);
	virtual ~QrVersionInformation() {}

	int getVersion() const;
	int getCodewordsCount() const;
	int getCodewordSize() const;
	Rect getVersionPosition1() const;
	Rect getVersionPosition2() const;
	Rect getTimerPattern1Position() const;
	Rect getTimerPattern2Position() const;

	void getFormatPosition1(vector<Rect> &formatPositions) const;
	void getFormatPosition2(vector<Rect> &formatPositions) const;
	void getFinderPatternPositions(vector<Rect> &finderPatterns) const;
	void getAlignmentPatternPositions(vector<Rect> &alignmentPatterns) const;
	void getOtherMaskPositions(vector<Rect> &maskPositions) const;
	void getDataMask(BitMatrix &mask) const;

	Size getQrBarcodeSize() const;
	bool operator!=(const QrVersionInformation &rhs) const;
	bool operator==(const QrVersionInformation &rhs) const;
	bool operator>(const QrVersionInformation &rhs) const;
	bool operator<(const QrVersionInformation &rhs) const;
	bool operator>=(const QrVersionInformation &rhs) const;
	bool operator<=(const QrVersionInformation &rhs) const;

	static QrVersionInformation fromImage(const Mat &image, const DetectedMarks &sortedDetectedMarks);
};

} /* namespace barcodes */
#endif /* QRVERSIONINFORMATION_H_ */

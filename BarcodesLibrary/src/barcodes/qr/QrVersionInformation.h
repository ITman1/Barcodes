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

#include "../DetectedMarks.h"
#include "../common/GridSampler.h"

namespace barcodes {

#define QR_SIZE(version) 17 + 4 * version

#define _COMMA ,
#define _SEMICOLON ;

#define _NUMBERS(num,name_space) VERSION_ ## num = num
#define _VERSIONS(num,name_space) static const QrVersionInformation(num)
#define _CLASSES(num,name_space) class VERSION_ ## num {\
public:\
	static const int VERSION_NUMBER = num;\
	static const cv::Point VERSION_POSITION1;\
	static const cv::Point VERSION_POSITION2;\
}
#define _CLASSES_DEFINE(num,name_space) \
		const cv::Point name_space ::VERSION_ ## num::VERSION_POSITION1 = \
		(num <= 6)? cv::Point(-1,-1) : cv::Point(0, QR_SIZE(num) - 11); \
		const cv::Point name_space ::VERSION_ ## num::VERSION_POSITION2 = \
		(num <= 6)? cv::Point(-1,-1) : cv::Point(QR_SIZE(num) - 11, 0);

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
	const static map<int, uint32_t> ENCODED_VERSIONS;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS = 7;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS = 40;
	const static int ALIGNMENT_PATTERNS_LOOKUP_TABLE[ALIGNMENT_PATTERNS_LOOKUP_TABLE_VERSIONS][ALIGNMENT_PATTERNS_LOOKUP_TABLE_CENTERS];

	static QrVersionInformation decodeVersion(BitMatrix &bitMatrix, GridSampler::FlowDirection bitsDirection);
	static bool correctEncodedVersion(uint32_t &encodedVersion);
public:
	DECLARE_VERSION_CLASSES
	const static QrVersionInformation INVALID_VERSION;
	const static Rect INVALID_POSITION;
	const static Size INVALID_VERSION_SIZE;

	QrVersionInformation(int version) : version(version) {}
	virtual ~QrVersionInformation() {}

	int getVersion();
	Rect getVersionPosition1();
	Rect getVersionPosition2();
	Rect getTimerPattern1Position();
	Rect getTimerPattern2Position();

	void getFormatPosition1(vector<Rect> &formatPositions);
	void getFormatPosition2(vector<Rect> &formatPositions);
	void getFinderPatternPositions(vector<Rect> &finderPatterns);
	void getAlignmentPatternPositions(vector<Rect> &alignmentPatterns);
	void getOtherMaskPositions(vector<Rect> &maskPositions);
	void getDataMask(BitMatrix &mask);

	Size getQrBarcodeSize();
	bool operator!=(const QrVersionInformation &rhs) const;
	bool operator==(const QrVersionInformation &rhs) const;

	static QrVersionInformation fromImage(const Mat &image, const DetectedMarks &sortedDetectedMarks);
};

} /* namespace barcodes */
#endif /* QRVERSIONINFORMATION_H_ */

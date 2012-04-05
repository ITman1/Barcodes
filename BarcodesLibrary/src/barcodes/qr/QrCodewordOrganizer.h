/*
 * QrCodewordOrganizer.h
 *
 *  Created on: 1.4.2012
 *      Author: Scotty
 */

#ifndef QRCODEWORDORGANIZER_H_
#define QRCODEWORDORGANIZER_H_

#include <map>
#include "QrVersionInformation.h"
#include "QrFormatInformation.h"

namespace barcodes {
using namespace cv;
using namespace std;

class QrVersionFormatCharacteristic {
public:
	QrVersionFormatCharacteristic(int p, int errCorrBlocks, int c, int k, int r)
	: p(p), errCorrBlocks(errCorrBlocks), c(c), k(k) , r(r) {}

	int p;
	int errCorrBlocks;
	int c;
	int k;
	int r;
};

class QrVersionFormatCharacteristics:public vector<QrVersionFormatCharacteristic> {
public:
	QrVersionFormatCharacteristics() {}
	QrVersionFormatCharacteristics(int count, ...);
	QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first);
	QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first, QrVersionFormatCharacteristic second);

	void insertCharacteristics(int count, ...);
};

class QrCodewordOrganizer {
private:
	typedef pair<int, QrFormatInformation::ErrorCorrectionLevel> MAP_KEY_TYPE;

	struct cmp_key {
		bool operator() (const MAP_KEY_TYPE& lhs, const MAP_KEY_TYPE& rhs) const {
			return (lhs.first < rhs.first) || (lhs.first == rhs.first && lhs.second < rhs.second);
		}
	};

	static const pair<MAP_KEY_TYPE, QrVersionFormatCharacteristics > QrCodewordOrganizer_mapping[];
	static const map<MAP_KEY_TYPE, QrVersionFormatCharacteristics, cmp_key > CODEWORD_CHARACTERISTICS;

	QrVersionInformation &version;
	QrFormatInformation &format;
	QrVersionFormatCharacteristics characteristics;
	int codewordSize;

	void bitArrayToCodewordArray(BitArray &bitArray, vector<int> &vec);
	void codewordArrayToBitArray(vector<int> &vec, BitArray &bitArray);
public:
	QrCodewordOrganizer(QrVersionInformation &version, QrFormatInformation &format);

	void getCharacteristics(QrVersionFormatCharacteristics &characteristics);

	void extractDataCodewords(BitArray &code, BitArray &extractedData);
	void extractErrorCorrectionCodewords(BitArray &code, BitArray &ecCodewords);
	void extractCodewords(BitArray &code, BitArray &codewords);

	void extractDataBlocks(BitArray &code, vector<BitArray> &blocks);
	void extractErrorCorrectionBlocks(BitArray &code, vector<BitArray> &blocks);
	void extractBlocks(BitArray &code, vector<BitArray> &blocks);

	void dataBlocksToDataCodewords(vector<BitArray> &blocks, BitArray &dataCodewords);
	void errorCorrectionBlocksToErrorCorrectionCodewords(vector<BitArray> &blocks, BitArray &ecCodewords);
	void blocksToCodewords(vector<BitArray> &blocks, BitArray &codewords);

	bool correctBlocks(vector<BitArray> &blocks);
};

} /* namespace barcodes */
#endif /* QRCODEWORDORGANIZER_H_ */

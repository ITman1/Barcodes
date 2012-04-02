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
	static const QrCodewordOrganizer INSTANCE;
	static const map<MAP_KEY_TYPE, QrVersionFormatCharacteristics, cmp_key > CODEWORD_CHARACTERISTICS;

	QrCodewordOrganizer() {}
public:
	virtual ~QrCodewordOrganizer() {}

	static QrCodewordOrganizer getInstance();
	void getCharacteristics(QrVersionInformation &version, QrFormatInformation &format, QrVersionFormatCharacteristics &characteristics);
	void extractDataCodewords(BitArray &code, QrVersionInformation &version, QrFormatInformation &format, BitArray &extractedData);
};

} /* namespace barcodes */
#endif /* QRCODEWORDORGANIZER_H_ */

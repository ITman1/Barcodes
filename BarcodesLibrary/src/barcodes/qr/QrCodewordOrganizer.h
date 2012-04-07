///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrCodewordOrganizer.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines QrCodewordOrganizer class that implements extracting
//             methods of the data/error correction blocks/codewords. Also
//             contains method that repairs the corrupted data blocks.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrCodewordOrganizer.h
 *
 * @brief Defines QrCodewordOrganizer class that implements extracting
 *        methods of the data/error correction blocks/codewords. Also
 *        contains method that repairs the corrupted data blocks.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef QRCODEWORDORGANIZER_H_
#define QRCODEWORDORGANIZER_H_

#include <map>
#include "QrVersionInformation.h"
#include "QrFormatInformation.h"

namespace barcodes {
using namespace cv;
using namespace std;

/**
 * One record of the characteristics.
 * See Table 9 — Error correction characteristics for QR Code 2005 (ISO 18004:2006)
 */
class QrVersionFormatCharacteristic {
public:
	QrVersionFormatCharacteristic(int p, int errCorrBlocks, int c, int k, int r)
	: p(p), errCorrBlocks(errCorrBlocks), c(c), k(k) , r(r) {}

	int p;				/**< Number of misdecode protection codewords */
	int errCorrBlocks;  /**< number of error correction blocks */
	int c;				/**< Total number of all codewords in the block */
	int k;				/**< Number of data codewords in the block */
	int r;				/**< Error correction capacity */
};

/**
 * Class groups characteristics for one version/format combination.
 * See Table 9 — Error correction characteristics for QR Code 2005 (ISO 18004:2006)
 */
class QrVersionFormatCharacteristics:public vector<QrVersionFormatCharacteristic> {
public:
	QrVersionFormatCharacteristics() {}

	/**
	 * Constructs characteristics for specified number of records.
	 *
	 * @param count Number of characteristics records.
	 */
	QrVersionFormatCharacteristics(int count, ...);

	/**
	 * Constructs characteristics only with one record.
	 *
	 * @param first Record of the characteristics.
	 */
	QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first);

	/**
	 * Constructs characteristics only with two records.
	 *
	 * @param first First record of the characteristics.
	 * @param second Second record of the characteristics.
	 */
	QrVersionFormatCharacteristics(QrVersionFormatCharacteristic first, QrVersionFormatCharacteristic second);

private:
	/**
	 * Inserts characteristics into vector.
	 *
	 * @param count Number of characteristics records.
	 */
	void insertCharacteristics(int count, ...);
};

/**
 * Class that contains characteristics table.
 * See Table 9 — Error correction characteristics for QR Code 2005 (ISO 18004:2006)
 *
 * Implements methods under this table which provides especially extraction of the blocks
 * and codewords from the bit array.
 */
class QrCodewordOrganizer {
private:
	/**
	 * Shortcut for map key type of the characteristics table.
	 */
	typedef pair<int, QrFormatInformation::ErrorCorrectionLevel> MAP_KEY_TYPE;

	/**
	 * Functor for comparing the records inside characteristics table.
	 */
	struct cmp_key {
		bool operator() (const MAP_KEY_TYPE& lhs, const MAP_KEY_TYPE& rhs) const {
			return (lhs.first < rhs.first) || (lhs.first == rhs.first && lhs.second < rhs.second);
		}
	};

	/**
	 * Helper array for the characteristics map table.
	 */
	static const pair<MAP_KEY_TYPE, QrVersionFormatCharacteristics > QrCodewordOrganizer_mapping[];

	/**
	 * Map array which maps the characteristics for specified version and format of the QR code.
	 */
	static const map<MAP_KEY_TYPE, QrVersionFormatCharacteristics, cmp_key > CODEWORD_CHARACTERISTICS;

	/**
	 * Version for which characteristics are revealed.
	 */
	QrVersionInformation version;

	/**
	 * Format for which characteristics are revealed.
	 */
	QrFormatInformation format;

	/**
	 * Characteristics record for passed version and format.
	 */
	QrVersionFormatCharacteristics characteristics;

	/**
	 * Size of the codeword for passed version.
	 */
	int codewordSize;

	/**
	 * Converts bit array to the byte array.
	 *
	 * @param bitArray Bit array to be converted.
	 * @param vec Vector which holds byte array.
	 */
	void bitArrayToCodewordArray(BitArray &bitArray, vector<int> &vec);

	/**
	 * Converts byte array to the vector.
	 *
	 * @param vec Vector that holds byte array.
	 * @param bitArray Output bit array.
	 */
	void codewordArrayToBitArray(vector<int> &vec, BitArray &bitArray);
public:
	/**
	 * Constructs characteristics for passed version and format.
	 *
	 * @param version Version of the QR code.
	 * @param format Format of the QR code.
	 */
	QrCodewordOrganizer(QrVersionInformation version, QrFormatInformation format);

	/**
	 * Returns characteristics for this version and format of the QR code.
	 *
	 * @param characteristics Characteristics for this format and version of the QR code.
	 */
	void getCharacteristics(QrVersionFormatCharacteristics &characteristics);

	/**
	 * Extracts data codewords from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted data codewords.
	 * @param extractedData Extracted data codewords.
	 */
	void extractDataCodewords(BitArray &code, BitArray &extractedData);

	/**
	 * Extracts error correction codewords from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted error correction codewords.
	 * @param ecCodewords Extracted error correction codewords.
	 */
	void extractErrorCorrectionCodewords(BitArray &code, BitArray &ecCodewords);

	/**
	 * Extracts codewords from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted codewords.
	 * @param codewords Extracted codewords.
	 */
	void extractCodewords(BitArray &code, BitArray &codewords);

	/**
	 * Extracts data blocks from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted data blocks.
	 * @param blocks Extracted data blocks.
	 */
	void extractDataBlocks(BitArray &code, vector<BitArray> &blocks);

	/**
	 * Extracts error correction blocks from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted error correction blocks.
	 * @param blocks Extracted error correction blocks.
	 */
	void extractErrorCorrectionBlocks(BitArray &code, vector<BitArray> &blocks);

	/**
	 * Extracts blocks from the code.
	 *
	 * @param code Bit array of the decoded code from which should be extracted blocks.
	 * @param blocks Extracted blocks.
	 */
	void extractBlocks(BitArray &code, vector<BitArray> &blocks);

	/**
	 * Converts data blocks to data codewords.
	 *
	 * @param blocks Data blocks.
	 * @param dataCodewords Data codewords.
	 */
	void dataBlocksToDataCodewords(vector<BitArray> &blocks, BitArray &dataCodewords);

	/**
	 * Converts error correction blocks to error correction codewords.
	 *
	 * @param blocks Error correction blocks.
	 * @param ecCodewords Error correction codewords.
	 */
	void errorCorrectionBlocksToErrorCorrectionCodewords(vector<BitArray> &blocks, BitArray &ecCodewords);

	/**
	 * Converts blocks to codewords.
	 *
	 * @param blocks Blocks to be converted.
	 * @param codewords Result Codewords.
	 */
	void blocksToCodewords(vector<BitArray> &blocks, BitArray &codewords);

	/**
	 * Corrects the blocks.
	 *
	 * @param blocks Blocks to be corrected.
	 * @return True if no correction was applied or correction finished with success, otherwise false.
	 */
	bool correctBlocks(vector<BitArray> &blocks);
};

} /* namespace barcodes */
#endif /* QRCODEWORDORGANIZER_H_ */

///////////////////////////////////////////////////////////////////////////////
// Project:    Barcodes Library
// File:       QrDecoder.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of QrDecoder class which implements decoder
//             of the QR codes in the image.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file QrDecoder.cpp
 *
 * @brief Defines members of QrDecoder class which implements decoder
 *        of the QR codes in the image.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>

#include "QrDecoder.h"
#include "QrVersionInformation.h"
#include "QrFormatInformation.h"
#include "../../common/miscellaneous.h"
#include "../../debug.h"
#include "bitdecoder/QrBitDecoder.h"
#include "QrCodewordOrganizer.h"
#include "perspcorners/PerspCornersFromLineSampling.h"
#include "perspcorners/PerspCornersFromAlignmentPattern.h"
#include "perspcorners/PerspCornersFromFinderPattern.h"

#define DEBUG_TAG "QrDecoder.cpp"

namespace barcodes {

static PerspCornersFromLineSampling perspCornersFromLineSampling;
static PerspCornersFromAlignmentPattern perspCornersFromAlignmentPattern;
static PerspCornersFromFinderPattern perspCornersFromFinderPattern;

/**
 * Instance of the QR decoder.
 */
const QrDecoder QrDecoder::DECODER_INSTANCE = QrDecoder();

/**
 * The size of the sampling grid for retrieving the data from the bit matrix.
 */
const Size QrDecoder::CODEWORD_SAMPLE_SIZE(2, 4);

/**
 * Decodes QR code on the image and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param dataSegments Result decoded data segments.
 * @param flags Flags used for detection and decoding.
 */
void QrDecoder::decode(Image &image, DataSegments &dataSegments, int flags) const {
	DetectedMarks detectedMarks;

	QrDetector::getInstance()->detect(image, detectedMarks, flags);

	map<int,int> parentContourOccurances;
	bool foundThreeOnSameParentLevel = false;
	int parentIndex = 0;;
	if (detectedMarks.size() >= 3) {

		// Tries to find marks which has the same parent
		// If it fails, it uses original detected marks

		for (unsigned int i = 0; i < detectedMarks.size(); i++) {
			if (parentContourOccurances.find(detectedMarks[i].variant) == parentContourOccurances.end()) {
				parentContourOccurances.insert(pair<int,int>(detectedMarks[i].variant, 1));
			} else {
				parentContourOccurances.at(detectedMarks[i].variant) += 1;

				if (parentContourOccurances.at(detectedMarks[i].variant) == 3) {
					foundThreeOnSameParentLevel = true;
					parentIndex = detectedMarks[i].variant;
					break;
				}
			}
		}


		if (foundThreeOnSameParentLevel) { // There are three marks on the same parent
			DetectedMarks _detectedMarks;
			for (unsigned int i = 0; i < detectedMarks.size(); i++) {
				if (detectedMarks[i].variant == parentIndex) {
					_detectedMarks.push_back(detectedMarks[i]);
				}
			}
			read_V1_40(image, dataSegments, _detectedMarks);
		} else { // There are no three marks on the same parent level, just try luck
			read_V1_40(image, dataSegments, detectedMarks);
		}
	}
}

/**
 * Returns instance of the QR decoder.
 *
 * @return Instance of the QR decoder.
 */
const QrDecoder *QrDecoder::getInstance() {
	return &DECODER_INSTANCE;
}

Image QrDecoder::lastProcessedImage() const {
	return Image(warpedImage, IMAGE_COLOR_GRAYSCALE);
}

/**
 * Decodes QR code of the versions 1-40 and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param binarized Binarized image by the values from the detection.
 * @param dataSegments Result decoded data segments.
 * @param detectedMarks The localization marks.
 * @param fun Functor for getting the perspective corners.
 * @param flags Flags used for detection and decoding.
 */
void QrDecoder::_read_V1_40(Image &image, Mat &binarized, DataSegments &dataSegments, DetectedMarks &detectedMarks, GetPerspCorners &perspCornersFunct) const {
	dataSegments.clear();
	dataSegments.flags = 0;

	vector<Point> corners;
	DetectedMarks _detectedMarks = detectedMarks;

	//>>> 1) GETTING THE FOUR CORNERS OF THE QR CODE FROM THE IMAGE
	perspCornersFunct.getPerspectiveCorners(image, binarized, _detectedMarks, corners);

	if (corners.size() < 4) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET FOUR CORNERS OF THE QR CODE!");
		return;
	}

#ifdef TARGET_DEBUG
	Image img;
#endif
	DEBUG_WRITE_IMAGE(
		(
				img = Image(image.clone(), IMAGE_COLOR_GRAYSCALE),
				cvtColor(img, img, CV_GRAY2RGB),
				line( img, corners[0],corners[0], CV_RGB(255, 0, 0), 5, 8, 0),
				line( img, corners[1],corners[1], CV_RGB(0, 255, 0), 5, 8, 0),
				line( img, corners[2],corners[2], CV_RGB(0, 0, 255), 5, 8, 0),
				line( img, corners[3],corners[3], CV_RGB(255, 255, 0), 5, 8, 0),
				"perspective_corners.jpg"
		), img);

	//>>> 2) PERSPECTIVE TRANSFORMATION OF THE QR CODE

	int warpPerspectiveSize = (binarized.cols > binarized.rows)? binarized.cols : binarized.rows;
	warpedImage = warpPerspective(image, corners, false, Size(warpPerspectiveSize, warpPerspectiveSize));
	/*Mat perspWarped;
	cv::GaussianBlur(image2, perspWarped, cv::Size(0, 0), 3);
	cv::addWeighted(image2, 1.5, perspWarped, -0.5, 0, perspWarped);*/
	warpedImage = QrDetector::binarize(warpedImage, QrDetector::FLAG_ADAPT_THRESH | QrDetector::FLAG_DISTANCE_NEAR);
	Mat transformation = getPerspectiveTransform(corners, Size(warpPerspectiveSize, warpPerspectiveSize));
	_detectedMarks.perspectiveTransform(transformation);
	DEBUG_WRITE_IMAGE("warped.jpg", warpedImage);

	//>>> 3) GETTING THE QR CODE VERSION FROM THE IMAGE

	QrVersionInformation versionInformation = QrVersionInformation::fromImage(warpedImage, _detectedMarks);

	if (versionInformation == QrVersionInformation::INVALID_VERSION) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET VERSION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "Processing version: %d", versionInformation.getVersion());

	//>>> 4) TRANSLATING THE IMAGE TO THE BIT MATRIX

	BitMatrix qrBitMatrix;
	BitMatrix::fromImage(warpedImage, versionInformation.getQrBarcodeSize(), qrBitMatrix);

	if (qrBitMatrix.data == NULL) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET QR BIT MATRIX!");
		return;
	}
	DEBUG_WRITE_BITMATRIX("data_unmasked.bmp", qrBitMatrix);

	//>>> 5) GETTING THE FORMAT INFORMATION FROM THE BIT MATRIX

	QrFormatInformation formatInformation = QrFormatInformation::fromBitMatrix(qrBitMatrix, versionInformation);

	if (formatInformation == QrFormatInformation::INVALID_FORMAT) {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET FORMAT INFORMATION!");
		return;
	}
	DEBUG_PRINT(DEBUG_TAG, "XOR DATA MASK: %d | ERROR CORRECTION LEVEL: %d", formatInformation.getXORDataMask(), formatInformation.getErrorCorrectionLevel());

	//>>> 6) BUILDING THE XOR DATA MASK AND MASKING THE QR CODE BIT MATRIX

	BitMatrix xorDataMask;
	formatInformation.buildXORDataMask(xorDataMask, versionInformation);

	qrBitMatrix.maskXOR(xorDataMask);
	DEBUG_WRITE_BITMATRIX("xor_mask.bmp", xorDataMask);
	DEBUG_WRITE_BITMATRIX("data_masked.bmp", qrBitMatrix);

	//>>> 7) SAMPLING THE QR CODE AND RETRIEVING BIT ARRAY CONTATINING DATA AND ERROR CORRECTION CODEWORDS

	GridSampler sampler(CODEWORD_SAMPLE_SIZE, GridSampler::LEFT_TOP, GridSampler::TOP_LEFT, false, true);
	BitMatrix dataMask;
	BitArray qrDataErrorBits;

	versionInformation.getDataMask(dataMask);
	qrBitMatrix.removeCol(6);
	dataMask.removeCol(6);
	sampler.sample(qrBitMatrix, qrDataErrorBits, &dataMask);
	DEBUG_PRINT(DEBUG_TAG, "READ DATA/ERROR BITS: %d", qrDataErrorBits.size());
	DEBUG_WRITE_BITMATRIX("data_mask.bmp", dataMask);

	//>>> 8) DIVIDES BITARRAY INTO CODEWORDS, RE-ORDERS AND RETURNS ORDERED BLOCKS CONTAINING EC PARITY BITS

	QrCodewordOrganizer codewordOrganizer(versionInformation, formatInformation);
	vector<BitArray> blocks;
	codewordOrganizer.extractBlocks(qrDataErrorBits, blocks);

	//>>> 9) PROCEEDS THE ERROR CORRECTION AND EXTRACTS CORECTED CODEWORDS

	BitArray codewords;
	if (!codewordOrganizer.correctBlocks(blocks)) {
		dataSegments.flags |= DataSegments::DATA_SEGMENTS_CORRUPTED;
		DEBUG_PRINT(DEBUG_TAG, "BLOCKS ARE CORRUPTED!");
	}
	codewordOrganizer.blocksToCodewords(blocks, codewords);

	//>>> 10) FINALLY DECODE THE DATA

	QrBitDecoder::getInstance().decode(codewords, dataSegments, versionInformation);
	DEBUG_PRINT(DEBUG_TAG, "DATA SEGMENT COUNT: %d", dataSegments.size());
	if (dataSegments.size() == 0)  {
		DEBUG_PRINT(DEBUG_TAG, "FAILED TO GET DATA SEGMENTS!");
		return;
	}

	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT length: %d", dataSegments[0].data.size());
	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT mode: %d", dataSegments[0].mode);
	DEBUG_PRINT(DEBUG_TAG, "FIRST DATA SEGMENT: %s", (dataSegments[0].data.size() > 0)? string((const char *)&dataSegments[0].data[0], dataSegments[0].data.size()).c_str() : NULL);
}

/**
 * Decodes QR code of the versions 1-40 and returns decoded data segments.
 *
 * @param image Image with the QR code.
 * @param dataSegments Result decoded data segments.
 * @param detectedMarks The localization marks.
 * @param flags Flags used for detection and decoding.
 */
void QrDecoder::read_V1_40(Image &image, DataSegments &dataSegments, DetectedMarks &detectedMarks, int flags) const {
	DataSegments bestReadSegments;

	Mat binarized = QrDetector::binarize(image, detectedMarks[2].flags);

	_read_V1_40(image, binarized, dataSegments, detectedMarks, perspCornersFromLineSampling);
	if ((!(dataSegments.flags & DataSegments::DATA_SEGMENTS_CORRUPTED)) && (dataSegments.size() > 0)) {
		return;
	}
	bestReadSegments = dataSegments;

	_read_V1_40(image, binarized, dataSegments, detectedMarks, perspCornersFromAlignmentPattern);
	if ((!(dataSegments.flags & DataSegments::DATA_SEGMENTS_CORRUPTED)) && (dataSegments.size() > 0)) {
		return;
	} else if (dataSegments.size() > 0) {
		bestReadSegments = dataSegments;
	}

	_read_V1_40(image, binarized, dataSegments, detectedMarks, perspCornersFromFinderPattern);
	if ((!(dataSegments.flags & DataSegments::DATA_SEGMENTS_CORRUPTED)) && (dataSegments.size() > 0)) {
		return;
	} else if (dataSegments.size() > 0) {
		bestReadSegments = dataSegments;
	}

	dataSegments = bestReadSegments;

}

} /* namespace barcodes */

///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       JNI_QRBarcodesLibrary.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Implements native interface methods which are called from java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file JNI_QRBarcodesLibrary.cpp
 *
 * @brief Implements native interface methods which are called from java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <vector>
#include <iostream>

#include <jni.h>
#include <barlib/debug.h>
#include <barlib/barcodes/qr/QrBarcode.h>

#include "wrappers/jDetectedMark.h"
#include "wrappers/jImage.h"
#include "wrappers/jDataSegments.h"

using namespace std;
using namespace jni;
using namespace barcodes;

#define DEBUG_TAG "JNI_QRBarcodesLibrary.cpp"

static Image img_detect;
static Image img_rgb_detect;

extern "C" {
	/**
	 * JNI method which is called for detection of the QR code.
	 */
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_detectQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		DEBUG_PRINT(DEBUG_TAG, "===== DETECT CALL =====");
		QrBarcode barcode;
		DetectedMarks detectedMarks;
		jImage _jImage(env, image);
		_jImage(img_detect);

		DEBUG_PRINT(DEBUG_TAG, "Image size: [%d : %d]", img_detect.cols, img_detect.rows);

		if (Image::convertColorFormat(img_detect, img_rgb_detect, IMAGE_COLOR_GRAYSCALE)) {
			DEBUG_WRITE_IMAGE(std::string("detect_image.jpg") , img_detect);
			// Detects the finder patterns in the image
			barcode.detect(img_rgb_detect, detectedMarks, QrDetector::FLAG_DISTANCE_MEDIUM | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_HIGH | QrDetector::FLAG_USE_HIERARCHY);

			// Allocating memory for java array
			int arrLength = detectedMarks.size();
			jobjectArray detectedMarksArr = env->NewObjectArray(arrLength, jDetectedMark::getJClass(env), NULL);

			// Adding detected marks into java array
			for (int i = 0; i < arrLength; i++) {
				jDetectedMark *detectedMark = new jDetectedMark(env, detectedMarks[i]);
				env->SetObjectArrayElement(detectedMarksArr, i, *detectedMark);
				delete detectedMark;
			}

			return detectedMarksArr;
		}

		return NULL;
	}
};

extern "C" {
	/**
	 * JNI method which is called for decoding the QR code.
	 */
	JNIEXPORT jobject JNICALL Java_com_qrcode_QrCodes_readQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		DEBUG_PRINT(DEBUG_TAG, "===== DECODE CALL =====");

		QrBarcode barcode;
		jImage _jImage(env, image);
		Image img_decode = _jImage;
		DataSegments dataSegments;

		DEBUG_PRINT(DEBUG_TAG, "Image size: [%d : %d]", img_decode.cols, img_decode.rows);

		if (img_decode.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {

			// Decodes the QR code from the image
			barcode.decode(img_decode, dataSegments, QrDetector::FLAG_ADAPT_THRESH | QrDetector::FLAG_QR_MARK_OUTER_FLOOD_FILL_REPAIR
					| QrDetector::DISTANCE_FLAGS | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_NORMAL);

			// If there are any data segments, return found QR code image
			if (dataSegments.size() > 0) {
		        vector<uchar> outImageBuffer;
		        vector<int> param = vector<int>(2);
		        param[0]=CV_IMWRITE_JPEG_QUALITY;
		        param[1]=40;

		        imencode(".jpg", QrDecoder::getInstance()->lastProcessedImage(), outImageBuffer,param);

		        jImage(env, image).setData(&outImageBuffer[0], outImageBuffer.size());
			}

			return jDataSegments(env, dataSegments);
		}

		return NULL;
	}
};

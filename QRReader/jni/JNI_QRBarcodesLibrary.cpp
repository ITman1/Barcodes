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

extern "C" {
	/**
	 * JNI method which is called for detection of the QR code.
	 */
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_detectQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		DEBUG_PRINT(DEBUG_TAG, "===== DETECT CALL =====");
		QrBarcode barcode;
		DetectedMarks detectedMarks;
		Image img = jImage(env, image);

		DEBUG_PRINT(DEBUG_TAG, "Image size: [%d : %d]", img.cols, img.rows);

		if (img.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {
			// Detects the finder patterns in the image
			barcode.detect(img, detectedMarks, QrDetector::FLAG_DISTANCE_MEDIUM | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_HIGH);

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
		Image img = jImage(env, image);
		DataSegments dataSegments;

		DEBUG_PRINT(DEBUG_TAG, "Image size: [%d : %d]", img.cols, img.rows);

		if (img.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {

			// Decodes the QR code from the image
			barcode.decode(img, dataSegments);

			return jDataSegments(env, dataSegments);
		}

		return NULL;
	}
};

#include <vector>
#include <iostream>

#include <jni.h>
#include <barlib/debug.h>
#include <barlib/barcodes/qr/QrBarcode.h>

#include "wrappers/jDetectedMark.h"
#include "wrappers/jImage.h"
#include "wrappers/jDataSegment.h"

using namespace std;
using namespace jni;
using namespace barcodes;

#define DEBUG_TAG "JNI_QRBarcodesLibrary.cpp"

extern "C" {
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_detectQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		QrBarcode barcode;
		DetectedMarks detectedMarks;
		Image img = jImage(env, image);

		DEBUG_PRINT(DEBUG_TAG, "Image color-format: [%d]", img.getColorFormat());
		DEBUG_PRINT(DEBUG_TAG, "Image elem-size: [%d]", CV_ELEM_SIZE(img.type()) * CV_ELEM_SIZE1(img.type()));
		DEBUG_PRINT(DEBUG_TAG, "Image size: [%d : %d]", img.cols, img.rows);
		if (img.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {
			//DEBUG_WRITE_IMAGE("last_image.jpg", img);
			barcode.detect(img, detectedMarks, QrDetector::FLAG_DISTANCE_MEDIUM | QrDetector::FLAG_QR_MARK_MATCH_TOLERANCE_HIGH);
			int arrLength = detectedMarks.size();

			jobjectArray detectedMarksArr = env->NewObjectArray(arrLength, jDetectedMark::getJClass(env), NULL);

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
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_readQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		QrBarcode barcode;
		Image img = jImage(env, image);
		vector<DataSegment> dataSegments;

		if (img.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {
			barcode.decode(img, dataSegments);
			int arrLength = dataSegments.size();

			jobjectArray dataSegmentsArr = env->NewObjectArray(arrLength, jDataSegment::getJClass(env), NULL);

			for (int i = 0; i < arrLength; i++) {
				jDataSegment *dataSegment = new jDataSegment(env, dataSegments[i]);DEBUG_PRINT(DEBUG_TAG, "AAAAAAAAAA");
				env->SetObjectArrayElement(dataSegmentsArr, i, *dataSegment);DEBUG_PRINT(DEBUG_TAG, "AAAAAAAAAA");
				delete dataSegment;DEBUG_PRINT(DEBUG_TAG, "AAAAAAAAAA");
			}

			return dataSegmentsArr;
		}

		return NULL;
	}
};

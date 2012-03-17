#include <vector>
#include <iostream>

#include <jni.h>
#include <barlib/barcodes/qr/QrBarcode.h>

#include "wrappers/jDetectedMark.h"
#include "wrappers/jImage.h"

using namespace std;
using namespace jni;
using namespace barcodes;

// TODO: DELETE
#include <android/log.h>
#define DEBUG_TAG "JNI_QRBarcodesLibrary.cpp"

extern "C" {
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_detectQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		QrBarcode barcode;
		vector<DetectedMark> detectedMarks;
		Image img = jImage(env, image);

		__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Image color-format: [%d]", img.getColorFormat());
		__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Image elem-size: [%d]", CV_ELEM_SIZE(img.type()) * CV_ELEM_SIZE1(img.type()));
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Image size: [%d : %d]", img.cols, img.rows);
		if (img.convertColorFormat(IMAGE_COLOR_GRAYSCALE)) {

			barcode.detect(img, detectedMarks, QrBarcode::FLAG_DISTANCE_MEDIUM);
			int arrLength = detectedMarks.size();

			jobjectArray detectedMarksArr = env->NewObjectArray(arrLength, jDetectedMark::getJClass(env), NULL);

			for (int i = 0; i < arrLength; i++) {
				jDetectedMark detectedMark = jDetectedMark(env, detectedMarks[i]);
				env->SetObjectArrayElement(detectedMarksArr, i, detectedMark);
			}

			return detectedMarksArr;
		}
	}
};

extern "C" {
	JNIEXPORT jbyteArray JNICALL Java_com_qrcode_QrCodes_readQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags)
	{

		/*jboolean isCopy;
		jbyte* buffer = env->GetByteArrayElements(image, &isCopy);
		jsize length = env->GetArrayLength(image);
		jsize j;

		for (j = 0; j < length; ++j)
		  buffer[j] = j;

		char *text = "skype:225154";
		int textLen = 13;

		jbyteArray qrCode = env->NewByteArray(textLen);
		env->SetByteArrayRegion(qrCode, 0, textLen, (jbyte*)text);

		if (isCopy == JNI_TRUE) {
			env->ReleaseArrayElements(image, buffer);
		}*/

		// DELETE THIS
		char *text = "skype:225154";
		int textLen = 13;

		jbyteArray qrCode = env->NewByteArray(textLen);
		env->SetByteArrayRegion(qrCode, 0, textLen, (jbyte*)text);
		return qrCode;
	}
};

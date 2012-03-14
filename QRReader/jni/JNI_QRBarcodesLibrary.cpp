#include <vector>
#include <iostream>

#include <jni.h>
#include <barlib/barcodes/qr/QrBarcode.h>

#include "wrappers/jDetectedMark.h"

using namespace std;
using namespace jni;
using namespace barcodes;

extern "C" {
	JNIEXPORT jobjectArray JNICALL Java_com_qrcode_QrCodes_detectQrCode(JNIEnv *env, jobject obj, jobject image, jint request, jint flags) {
		jboolean isCopy;
		QrBarcode barcode;
		vector<DetectedMark> detectedMarks;
		jbyte* passedImgData = env->GetByteArrayElements(image, &isCopy);
		jsize  passedImgLength = env->GetArrayLength(image);

		Image passedImg(passedImgLength, CV_16U, (void *)passedImgData);
		Image localImg;

		cvtColor(passedImg, localImg, CV_YCrCb2RGB);
		cvtColor(localImg, localImg, CV_RGB2GRAY);

		barcode.detect(image, detectedMarks);

		int arrLength = detectedMarks.size();
		jobjectArray detectedMarksArr = newObjectArray(jDetectedMark.getJClass(), arrLength, jDetectedMark());
		for (int i = 0; i < arrLength; i++) {
			jDetectedMark detectedMark = jDetectedMark(detectedMarks[i]);
			env->SetObjectArrayElement(detectedMark.getJObject(), i, detectedMark);
		}

		if (isCopy == JNI_TRUE) {
			env->ReleaseArrayElements(image, passedImgData);
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

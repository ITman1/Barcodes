///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jImage.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the the wrapper for accessing the members
//             of the objects of Image class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jImage.cpp
 *
 * @brief Defines members of the wrapper for accessing the members of the objects
 *        of Image class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <cstring>
#include <algorithm>

#include "jSize.h"
#include "jImage.h"

#include <barlib/debug.h>
#define DEBUG_TAG "jImage.cpp"

using namespace jni;
using namespace std;

const string jImage::CLASS_NAME = "com/qrcode/QrCodes$Image";

jImage::jImage(JNIEnv *env) : JNIWrapper(env, CLASS_NAME), dataArr(NULL), data_ptr(NULL) {}

jImage::jImage(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject), dataArr(NULL), data_ptr(NULL) {}

jImage::jImage(JNIEnv *env, Image image) : JNIWrapper(env, CLASS_NAME), dataArr(NULL), data_ptr(NULL) {
	setColorFormat(image.getColorFormat());
	setData(image.data, image.rows * image.cols * CV_ELEM_SIZE(image.type()) * CV_ELEM_SIZE1(image.type()));
	setSize(Size(image.cols, image.rows));
}

jImage::~jImage() {
	releaseDataReference();
}

void jImage::setColorFormat(int colorFormat) {
	setIntField(this, "colorFormat", colorFormat);
}

void jImage::setSize(Size size) {
	jSize _size(env, size);
	setObjectField(this, "size", &_size);
}

void jImage::setCompressed(bool compressed) {
	setBooleanField(this, "compressed", compressed);
}

void jImage::setData(uchar *data, int length) {
	jbyteArray dataArray = env->NewByteArray(length);
	env->SetByteArrayRegion(dataArray, 0, length, (jbyte*)data);
	setByteArray(this, "data", dataArray);
	env->DeleteLocalRef(dataArray);
}

void jImage::getData(ByteArray &data) {
	data.clear();

	jbyteArray dataArr = getByteArray(this, "data");
	jsize length = env->GetArrayLength(dataArr);
	jbyte *data_ptr = env->GetByteArrayElements(dataArr, NULL);

	if (data_ptr != NULL) {
		data.resize(length);
		copy(data_ptr, data_ptr + length, data.begin());
		env->ReleaseByteArrayElements(dataArr, data_ptr, JNI_ABORT);
	} else {
		data.clear();
	}

	env->DeleteLocalRef(dataArr);
}

int jImage::getColorFormat() {
	return getIntField(this, "colorFormat");
}

Size jImage::getSize() {
	jobject size = getObjectField(this, jSize::getJClass(env), "size");
	return (size != NULL)? jSize(env, size) : Size();
}

bool jImage::getCompressed() {
	return getBooleanField(this, "compressed");
}

void jImage::releaseDataReference() {
	if (dataArr != NULL && data_ptr != NULL) {
		env->ReleaseByteArrayElements(dataArr, data_ptr, JNI_ABORT);
	}
}

jImage::operator Image() {
	releaseDataReference();
	dataArr = getByteArray(this, "data");
	jsize length = env->GetArrayLength(dataArr);
	data_ptr = env->GetByteArrayElements(dataArr, JNI_FALSE);

	int format = getColorFormat();
	bool compressed = getCompressed();
	Size size = getSize();
	DEBUG_PRINT(DEBUG_TAG, "Compressed: %d", compressed);
	DEBUG_PRINT(DEBUG_TAG, "Data length: %d", data.size());
	DEBUG_PRINT(DEBUG_TAG, "Image size: %d : %d", size.width, size.height);

	if (compressed) {
		return Image::fromByteArrayGrayscale(data_ptr, length);
	} else {
		return Image(Image(size.height * 3/2, size.width, (void *)data_ptr, format), format);
	}
}

jclass jImage::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

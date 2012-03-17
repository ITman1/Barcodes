#include <cstring>
#include <algorithm>

#include "jSize.h"
#include "jImage.h"

using namespace jni;
using namespace std;

const string jImage::CLASS_NAME = "com/qrcode/QrCodes$Image";

// TODO: DELETE
#include <android/log.h>
#define DEBUG_TAG "jImage.cpp"

jImage::jImage(JNIEnv *env) : JNIWrapper(env, CLASS_NAME) {}

jImage::jImage(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject) {}

jImage::jImage(JNIEnv *env, Image image) : JNIWrapper(env, CLASS_NAME) {
	setColorFormat(image.getColorFormat());
	setData(image.data, image.rows * image.cols * CV_ELEM_SIZE(image.type()) * CV_ELEM_SIZE1(image.type()));
	setSize(Size(image.cols, image.rows));
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
	jboolean isCopy;
	jbyteArray dataArr = getByteArray(this, "data");
	jbyte *data_ptr = env->GetByteArrayElements(dataArr, &isCopy);
	jsize length = env->GetArrayLength(dataArr);
	data.resize(length);
	copy(data_ptr, data_ptr + length, data.begin());

	if (isCopy == JNI_TRUE) {
		env->ReleaseByteArrayElements(dataArr, data_ptr, 0);
	}

	env->DeleteLocalRef(dataArr);
}

int jImage::getColorFormat() {
	return getIntField(this, "colorFormat");
}

Size jImage::getSize() {
	return jSize(env, getObjectField(this, jSize::getJClass(env), "size"));
}

bool jImage::getCompressed() {
	return getBooleanField(this, "compressed");
}

jImage::operator Image() {
	ByteArray data;
	getData(data);
	int format = getColorFormat();
	int compressed = getCompressed();
	Size size = getSize();

	if (compressed) {
		__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Image decoding...");
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Size: [%d : %d]", size.width, size.height);
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Data length: [%d]", data.size());
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Color format: [%d]", format);
		return Image(imdecode(Mat(data), 0), IMAGE_COLOR_GRAYSCALE);
	} else  {
		__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Image assigning...");
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Size: [%d : %d]", size.width, size.height);
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Data length: [%d]", data.size());
	    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Color format: [%d]", format);
		return Image(Image(size.height * 3/2, size.width, (void *)&data[0], format).clone(), format);
	}
}

jclass jImage::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

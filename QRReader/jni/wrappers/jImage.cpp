#include <cstring>

#include "jImage.h"

using namespace jni;
using namespace std;

const string jSize::CLASS_NAME = "com/qrcode/QrCodes/Image";

jImage::jImage(JNIEnv *env) {
	JNIWrapper(env, CLASS_NAME);
}

jImage::jImage(JNIEnv *env, jobject jObject) {
	JNIWrapper(env, jObject);
}

jImage::jImage(JNIEnv *env, Image image) {
	JNIWrapper(env, CLASS_NAME);
	setImageFormat(image.getImageFormat());
	setData(image.data);
	setSize(Size(image.cols, image.rows));
}

void jImage::setImageFormat(int imageFormat) {
	setIntField(this, "imageFormat", imageFormat);
}

void jImage::setSize(Size size) {
	setObjectField(this, "size", jSize(env, size));
}

void jImage::setData(uchar *data, int length) {
	jbyteArray dataArray = env->NewByteArray(length);
	env->SetByteArrayRegion(data, 0, length, (jbyte*)data);
	setObjectField(this, "data", dataArray);
}

ByteArray jImage::getData() {
	jboolean isCopy;
	jbyteArray dataArr = (jbyteArray)getObjectField(this, "data");
	jbyte* jniRetData = env->GetByteArrayElements(dataArr, &isCopy);
	jsize length = env->GetArrayLength(image);
	char *data;

	memcpy (data, jniRetData, length);
	if (isCopy == JNI_TRUE) {
		env->ReleaseArrayElements(dataArr, jniRetData);
	}

	return data;
}

int jImage::getImageFormat() {
	return getIntField(this, "imageFormat");
}

Size jImage::getSize() {
	return jSize(getObjectField(this, "size"));
}

jSize::operator Image() {
	Image
	return Size(setWidth(), setHeight());
}

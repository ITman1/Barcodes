#include "jSize.h"

using namespace jni;
using namespace std;

const string jSize::CLASS_NAME = "com/qrcode/QrCodes$Size";

jSize::jSize(JNIEnv *env) : JNIWrapper(env, CLASS_NAME) {}

jSize::jSize(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject) {}

jSize::jSize(JNIEnv *env, Size size) : JNIWrapper(env, CLASS_NAME) {
	setWidth(size.width);
	setHeight(size.height);
}

void jSize::setWidth(int width) {
	setIntField(this, "width", width);
}

void jSize::setHeight(int height) {
	setIntField(this, "height", height);
}

int jSize::getWidth() {
	return getIntField(this, "width");
}

int jSize::getHeight() {
	return getIntField(this, "height");
}

jSize::operator Size() {
	return Size(getWidth(), getHeight());
}

jclass jSize::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

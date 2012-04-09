///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jSize.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the the wrapper for accessing the members
//             of the objects of Size class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jSize.cpp
 *
 * @brief Defines members of the wrapper for accessing the members of the objects
 *        of Size class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

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

#include "jPoint.h"

using namespace jni;
using namespace std;

const string jPoint::CLASS_NAME = "com/qrcode/QrCodes/Point";

jPoint::jPoint(JNIEnv *env) {
	JNIWrapper(env, CLASS_NAME);
}

jPoint::jPoint(JNIEnv *env, jobject jObject) {
	JNIWrapper(env, jObject);
}

jPoint::jPoint(JNIEnv *env, Point point) {
	JNIWrapper(env, CLASS_NAME);
	setX(point.x);
	setY(point.y);
}

void jPoint::setX(int x) {
	setIntField(this, "x", x);
}

void jPoint::setY(int y) {
	setIntField(this, "y", y);
}

int jPoint::getX() {
	return getIntField(this, "x");
}

int jPoint::getY() {
	return getIntField(this, "y");
}

jPoint::operator Point() {
	return Point(getX(), getY());
}

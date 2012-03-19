#include "jDetectedMark.h"

using namespace jni;
using namespace std;

const string jDetectedMark::CLASS_NAME = "com/qrcode/QrCodes$DetectedMark";

jDetectedMark::jDetectedMark(JNIEnv *env) : JNIWrapper(env, CLASS_NAME) {}

jDetectedMark::jDetectedMark(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject) {}

jDetectedMark::jDetectedMark(JNIEnv *env, DetectedMark detectedMark) : JNIWrapper(env, CLASS_NAME) {
	setPoints(detectedMark.points);
	setMatch(detectedMark.match);
	setFlags(detectedMark.flags);
}

vector<Point> jDetectedMark::getPoints() {
	vector<Point> points;

	int arrLength = getArrayLength(this, jPoint::getJClass(env), "points");
	for (int i = 0; i < arrLength; i++) {
		points.push_back(jPoint(env, getObjectArrayElement(this, jPoint::getJClass(env), "points", i)));
	}

	return points;
}

void jDetectedMark::setPoints(vector<Point> points) {
	int arrLength = points.size();
	jobjectArray pointsArray = env->NewObjectArray(arrLength, jPoint::getJClass(env), NULL);

    setObjectArray(this, jPoint::getJClass(env), "points", pointsArray);

	for (int i = 0; i < arrLength; i++) {
		jPoint point = jPoint(env, points[i]);
		setObjectArrayElement(this, jPoint::getJClass(env), "points", i, point);
	}

	env->DeleteLocalRef(pointsArray);
}

double jDetectedMark::getMatch() {
	return getDoubleField(this, "match");
}

void jDetectedMark::setMatch(double match) {
	setDoubleField(this, "match", match);
}

int jDetectedMark::getFlags() {
	return getIntField(this, "flags");
}

void jDetectedMark::setFlags(int flags) {
	setIntField(this, "flags", flags);
}

jDetectedMark::operator DetectedMark() {
	DetectedMark detectedMark;
	detectedMark.flags = getFlags();
	detectedMark.points = getPoints();
	detectedMark.match = getMatch();

	return detectedMark;
}

jclass jDetectedMark::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

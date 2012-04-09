///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jDataSegments.cpp
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines members of the the wrapper for accessing the members
//             of the objects of DataSegments class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jDataSegments.cpp
 *
 * @brief Defines members of the wrapper for accessing the members of the objects
 *        of DataSegments class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <barlib/debug.h>

#include "jDataSegment.h"
#include "jDataSegments.h"

using namespace jni;
using namespace std;

const string jDataSegments::CLASS_NAME = "com/qrcode/QrCodes$DataSegments";

jDataSegments::jDataSegments(JNIEnv *env) : JNIWrapper(env, CLASS_NAME) {}

jDataSegments::jDataSegments(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject) {}

jDataSegments::jDataSegments(JNIEnv *env, DataSegments &dataSegments) : JNIWrapper(env, CLASS_NAME) {
	setSegments(dataSegments);
	setFlags(dataSegments.flags);
}

vector<DataSegment> jDataSegments::getSegments() {
	vector<DataSegment> segments;

	int arrLength = getArrayLength(this, jDataSegment::getJClass(env), "segments");
	for (int i = 0; i < arrLength; i++) {
		segments.push_back(jDataSegment(env, getObjectArrayElement(this, jDataSegment::getJClass(env), "segments", i)));
	}

	return segments;
}

void jDataSegments::setSegments(vector<DataSegment> &segments) {
	int arrLength = segments.size();
	jobjectArray dataSegmentsArr = env->NewObjectArray(arrLength, jDataSegment::getJClass(env), NULL);
	setObjectArray(this, jDataSegment::getJClass(env), "segments", dataSegmentsArr);

	for (int i = 0; i < arrLength; i++) {
		jDataSegment *dataSegment = new jDataSegment(env, segments[i]);
		env->SetObjectArrayElement(dataSegmentsArr, i, *dataSegment);
		delete dataSegment;
	}

	env->DeleteLocalRef(dataSegmentsArr);
}

int jDataSegments::getFlags() {
	return getIntField(this, "flags");
}

void jDataSegments::setFlags(int flags) {
	setIntField(this, "flags", flags);
}

jDataSegments::operator DataSegments() {
	DataSegments dataSegments;

	dataSegments.flags = getFlags();
	vector<DataSegment> segments = getSegments();
	dataSegments.insert(dataSegments.end(), segments.begin(), segments.end());

	return dataSegments;
}

jclass jDataSegments::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

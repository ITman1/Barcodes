#include "jDataSegment.h"

using namespace jni;
using namespace std;

const string jDataSegment::CLASS_NAME = "com/qrcode/QrCodes$DataSegment";

jDataSegment::jDataSegment(JNIEnv *env) : JNIWrapper(env, CLASS_NAME) {}

jDataSegment::jDataSegment(JNIEnv *env, jobject jObject) : JNIWrapper(env, jObject) {}

jDataSegment::jDataSegment(JNIEnv *env, DataSegment &dataSegment) : JNIWrapper(env, CLASS_NAME) {
	setData(dataSegment.data);
	setMode(dataSegment.mode);
	setRemainderBits(dataSegment.remainderBits);
	setFlags(dataSegment.flags);
}

ByteArray jDataSegment::getData() {
	ByteArray data;
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

	return data;
}

void jDataSegment::setData(ByteArray &data) {
	jbyteArray dataArray = env->NewByteArray(data.size());
	env->SetByteArrayRegion(dataArray, 0, data.size(), (jbyte*)&data[0]);
	setByteArray(this, "data", dataArray);
	env->DeleteLocalRef(dataArray);
}

int jDataSegment::getMode() {
	return getIntField(this, "mode");
}

void jDataSegment::setMode(int mode) {
	setIntField(this, "mode", mode);
}

size_t jDataSegment::getRemainderBits() {
	return getIntField(this, "remainderBits");
}

void jDataSegment::setRemainderBits(size_t remainderBits) {
	setIntField(this, "remainderBits", remainderBits);
}

int jDataSegment::getFlags() {
	return getIntField(this, "flags");
}

void jDataSegment::setFlags(int flags) {
	setIntField(this, "flags", flags);
}

jDataSegment::operator DataSegment() {
	DataSegment dataSegment;
	dataSegment.flags = getFlags();
	dataSegment.mode = getMode();
	dataSegment.data = getData();
	dataSegment.remainderBits = getRemainderBits();

	return dataSegment;
}

jclass jDataSegment::getJClass(JNIEnv *env) {
	return env->FindClass(CLASS_NAME.c_str());
}

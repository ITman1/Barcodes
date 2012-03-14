#include <jni.h>

#include "JNIWrapper.h"

namespace jni {

inline jmethodID getConstructor(JNIWrapper *jniWrapper, string signature = "()V") {
	return env->GetMethodID(cls, "<init>", signature);
}

inline jfieldID getFID(JNIWrapper *jniWrapper, string &name, string &signature) {
	JNIEnv *env = jniWrapper->getEnv();
	return env->GetFieldID(env->getJClass(), name, signature);
}

inline int getIntField(JNIWrapper *jniWrapper, string &name) {
	JNIEnv *env = jniWrapper->getEnv();
	return env->GetIntField(env->getJObject(), getFID(jniWrapper, name, "I"));
}

inline void setIntField(JNIWrapper *jniWrapper, string &name, int value) {
	JNIEnv *env = jniWrapper->getEnv();
	env->SetIntField(env->getJObject(), getFID(jniWrapper, name, "I"), value);
}

inline double getDoubleField(JNIWrapper *jniWrapper, string &name) {
	JNIEnv *env = jniWrapper->getEnv();
	return env->GetDoubleField(env->getJObject(), getFID(jniWrapper, name, "D"));
}

inline void setDoubleField(JNIWrapper *jniWrapper, string &name, double value) {
	JNIEnv *env = jniWrapper->getEnv();
	env->SetDoubleField(env->getJObject(), getFID(jniWrapper, name, "D"), value);
}

inline jobject getObjectField(JNIWrapper *jniWrapper, string &name) {
	JNIEnv *env = jniWrapper->getEnv();
	string classSignature = jniWrapper->getClassSignature();
	return env->GetObjectField(env->getJObject(), getFID(jniWrapper, name, classSignature));
}

inline void setObjectField(JNIWrapper *jniWrapper, string &name, jobject value) {
	JNIEnv *env = jniWrapper->getEnv();
	string classSignature = jniWrapper->getClassSignature();
	env->SetObjectField(env->getJObject(), getFID(jniWrapper, name, classSignature), value);
}

inline jsize getArrayLength(JNIWrapper *jniWrapper, string &name) {
	JNIEnv *env = jniWrapper->getEnv();
	jarray objectArray = getObjectField(jniWrapper, name);
	return env->GetArrayLength(objectArray);
}

inline jobject getObjectArrayElement(JNIWrapper *jniWrapper, string &name, jsize index) {
	JNIEnv *env = jniWrapper->getEnv();
	jobjectArray objectArray = getObjectField(jniWrapper, name);
	return env->GetObjectArrayElement(objectArray, index);
}

inline void setObjectArrayElement(JNIWrapper *jniWrapper, string &name, jsize index, jobject value) {
	JNIEnv *env = jniWrapper->getEnv();
	jobjectArray objectArray = getObjectField(jniWrapper, name);
	env->SetObjectArrayElement(objectArray, index, value);
}

inline jobjectArray newObjectArray(jclass objectClass, jsize length, jobject initialElement) {
	return jniWrapper->getEnv()->NewObjectArray(length, objectClass, initialElement);
}

}

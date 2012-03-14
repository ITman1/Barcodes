#include "JNIWrapper.h"

using namespace jni;

JNIWrapper::JNIWrapper(JNIEnv *env, string className, string signature, ...) {
	this->env = env;
	jclass cls = env->FindClass(env, className);
	jmethodID constructor = getConstructor(this, signature);

    va_list args;
    va_start(args, format);
	this->jObject = env->NewObject(cls, constructor, args);
    va_end(args);
}

JNIWrapper::JNIWrapper(JNIEnv *env, jObject jObject) {
	this->env = env;
	this->jObject = jObject;
}

#ifndef JNI_JNIWRAPPER_H_
#define JNI_JNIWRAPPER_H_

#include <algorithm>
#include <string>
#include <jni.h>

using namespace std;

namespace jni {

class JNIWrapper {
public:
	JNIWrapper(JNIEnv *env, string className, string signature = "()V", ...) : localJObject(true) {
		this->env = env;
		this->cls = env->FindClass(className.c_str());
		jmethodID constructor = getConstructor(this, signature);

	    va_list args;
	    va_start(args, signature);
		this->jObject = env->NewObject(cls, constructor, args);
	    va_end(args);
	}

	JNIWrapper(JNIEnv *env, jobject jObject) : localJObject(false) {
		this->env = env;
		this->jObject = jObject;
		this->cls = env->GetObjectClass(jObject);
	}

	virtual ~JNIWrapper() {
		if (localJObject) {
			env->DeleteLocalRef(jObject);
		}
	}

	inline JNIEnv* getEnv() {
		return env;
	}

	inline jobject getJObject() {
		return jObject;
	}

	inline jclass getJClass() {
		return cls;
	}

	inline string getClassSignature() {
		return getClassSignature(env, cls);
	}

	inline operator jobject() {
		return jObject;
	}

	static inline jmethodID getConstructor(JNIWrapper *objWrapper, string signature = "()V") {
		return objWrapper->getEnv()->GetMethodID(
			objWrapper->getJClass(), "<init>", signature.c_str()
		);
	}

	static inline jfieldID getFID(JNIWrapper *objWrapper, const string &name, string signature) {
		return objWrapper->getEnv()->GetFieldID(
			objWrapper->getJClass(), name.c_str(), signature.c_str()
		);
	}

	static inline int getIntField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetIntField(
			objWrapper->getJObject(), getFID(objWrapper, name, "I")
		);
	}

	static inline void setIntField(JNIWrapper *objWrapper, const string &name, int value) {
		objWrapper->getEnv()->SetIntField(
			objWrapper->getJObject(), getFID(objWrapper, name, "I"), value
		);
	}

	static inline bool getBooleanField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetBooleanField(
			objWrapper->getJObject(), getFID(objWrapper, name, "Z")
		);
	}

	static inline void setBooleanField(JNIWrapper *objWrapper, const string &name, int value) {
		objWrapper->getEnv()->SetBooleanField(
			objWrapper->getJObject(), getFID(objWrapper, name, "Z"), value
		);
	}

	static inline double getDoubleField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetDoubleField(
			objWrapper->getJObject(), getFID(objWrapper, name, "D")
		);
	}

	static inline void setDoubleField(JNIWrapper *objWrapper, const string &name, double value) {
		objWrapper->getEnv()->SetDoubleField(
			objWrapper->getJObject(), getFID(objWrapper, name, "D"), value
		);
	}

	static inline jobject getObjectField(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		return env->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, JNIWrapper::getClassSignature(env, elemClass))
		);
	}

	static inline void setObjectField(JNIWrapper *objWrapper, jclass elemClass, const string &name, jobject value) {
		JNIEnv *env = objWrapper->getEnv();
		env->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, JNIWrapper::getClassSignature(env, elemClass)), value
		);
	}

	static inline void setObjectField(JNIWrapper *objWrapper, const string &name, JNIWrapper *value) {
		objWrapper->getEnv()->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, value->getClassSignature()), *value
		);
	}

	static inline jbyteArray getByteArray(JNIWrapper *objWrapper, const string &name) {
		return (jbyteArray)objWrapper->getEnv()->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[B")
		);
	}

	static inline void setByteArray(JNIWrapper *objWrapper, const string &name, jbyteArray byteArray) {
		objWrapper->getEnv()->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[B"), byteArray
		);
	}

	static inline jobjectArray getObjectArray(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		return (jobjectArray)env->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[" + JNIWrapper::getClassSignature(env, elemClass))
		);
	}

	static inline void setObjectArray(JNIWrapper *objWrapper, jclass elemClass, const string &name, jobjectArray value) {
		JNIEnv *env = objWrapper->getEnv();
		env->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[" + JNIWrapper::getClassSignature(env, elemClass)), value
		);
	}

	static inline jsize getArrayLength(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		jsize length = env->GetArrayLength(objectArray);
		env->DeleteLocalRef(objectArray);
		return length;
	}

	static inline jobject getObjectArrayElement(JNIWrapper *objWrapper, jclass elemClass, const string &name, jsize index) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		jobject elem = env->GetObjectArrayElement(objectArray, index);
		env->DeleteLocalRef(objectArray);
		return elem;
	}

	static inline void setObjectArrayElement(JNIWrapper *objWrapper, jclass elemClass, const string &name, jsize index, jobject value) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		env->SetObjectArrayElement(objectArray, index, value);
		env->DeleteLocalRef(objectArray);
	}

	static inline string getClassSignature(JNIEnv *env, jclass cls) {
		jclass classClass = env->FindClass("java/lang/Class");
		jmethodID nameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");

		jstring jClassName = (jstring)env->CallObjectMethod(cls, nameMethod);

		const char* strClassName = env->GetStringUTFChars(jClassName , NULL);
		string className = string(strClassName);

		env->ReleaseStringUTFChars(jClassName , strClassName);

		replace(className.begin(), className.end(), '.', '/');
		return "L" + className + ";";
	}

protected:
	JNIEnv *env;
	jobject jObject;
	jclass cls;
	bool localJObject;
};

}

#endif

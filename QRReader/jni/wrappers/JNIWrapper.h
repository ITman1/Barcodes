///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       JNIWrapper.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines base class for all wrappers which wraps java object and
//             provides interface for accessing java object members from C++.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file JNIWrapper.h
 *
 * @brief Defines base class for all wrappers which wraps java object and
 *        provides interface for accessing java object members from C++.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JNIWRAPPER_H_
#define JNI_JNIWRAPPER_H_

#include <algorithm>
#include <string>
#include <jni.h>

using namespace std;

namespace jni {

/**
 * Class of the base wrapper from should be derived all wrappers.
 */
class JNIWrapper {
public:
	/**
	 * Constructs wrapper and new java object.
	 *
	 * @param env Pointer to the JNI environment.
	 * @param className Full class name of the java class which wraps this wrapper.
	 * @param signature Java signature of the constructor by which will be
	 *        constructed java object inside this constructor.
	 * @param ... Arguments which should be passed into constructor.
	 */
	JNIWrapper(JNIEnv *env, string className, string signature = "()V", ...) : localJObject(true) {
		this->env = env;
		this->cls = env->FindClass(className.c_str());
		jmethodID constructor = getConstructor(this, signature);

	    va_list args;
	    va_start(args, signature);
		this->jObject = env->NewObject(cls, constructor, args);
	    va_end(args);
	}

	/**
	 * Constructs wrapper for already existing java object.
	 *
	 * @param env Pointer to the JNI environment.
	 * @param jObject Already existing java object.
	 */
	JNIWrapper(JNIEnv *env, jobject jObject) : localJObject(false) {
		this->env = env;
		this->jObject = jObject;
		this->cls = env->GetObjectClass(jObject);
	}

	/**
	 * Releases local reference if was the object allocated locally.
	 */
	virtual ~JNIWrapper() {
		if (localJObject) {
			env->DeleteLocalRef(jObject);
		}
	}

	/**
	 * Returns pointer to the JNI environment.
	 *
	 * @return Pointer to the JNI environment.
	 */
	inline JNIEnv* getEnv() {
		return env;
	}

	/**
	 * Returns java object which wraps this wrapper.
	 *
	 * @return Java object which wraps this wrapper.
	 */
	inline jobject getJObject() {
		return jObject;
	}

	/**
	 * Returns java class object of the wrapped object.
	 *
	 * @return Java class object of the wrapped object.
	 */
	inline jclass getJClass() {
		return cls;
	}

	/**
	 * Returns signature of the wrapped java object.
	 *
	 * @return Signature of the wrapped java object.
	 */
	inline string getClassSignature() {
		return getClassSignature(env, cls);
	}

	/**
	 * Reveals java object.
	 *
	 * @return Wrapped java object.
	 */
	inline operator jobject() {
		return jObject;
	}

	/**
	 * Returns method ID of the constructor for some specified object.
	 *
	 * @param objWrapper Wrapper of the object which wraps object of which constructor should be returned.
	 * @param signature Signature of the constructor if is not implicit.
	 * @return Method ID of the demanded constructor.
	 */
	static inline jmethodID getConstructor(JNIWrapper *objWrapper, string signature = "()V") {
		return objWrapper->getEnv()->GetMethodID(
			objWrapper->getJClass(), "<init>", signature.c_str()
		);
	}

	/**
	 * Returns field ID of the object member variable.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @param signature Signature of the variable.
	 * @return Field ID of the demanded variable.
	 */
	static inline jfieldID getFID(JNIWrapper *objWrapper, const string &name, string signature) {
		return objWrapper->getEnv()->GetFieldID(
			objWrapper->getJClass(), name.c_str(), signature.c_str()
		);
	}

	/**
	 * Returns value of the integer field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @return Integer value of the variable.
	 */
	static inline int getIntField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetIntField(
			objWrapper->getJObject(), getFID(objWrapper, name, "I")
		);
	}

	/**
	 * Sets value of the integer field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @param value New value which should by which should be set this variable.
	 */
	static inline void setIntField(JNIWrapper *objWrapper, const string &name, int value) {
		objWrapper->getEnv()->SetIntField(
			objWrapper->getJObject(), getFID(objWrapper, name, "I"), value
		);
	}

	/**
	 * Returns value of the boolean field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @return Boolean value of the variable.
	 */
	static inline bool getBooleanField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetBooleanField(
			objWrapper->getJObject(), getFID(objWrapper, name, "Z")
		);
	}

	/**
	 * Sets value of the boolean field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @param value New value which should by which should be set this variable.
	 */
	static inline void setBooleanField(JNIWrapper *objWrapper, const string &name, int value) {
		objWrapper->getEnv()->SetBooleanField(
			objWrapper->getJObject(), getFID(objWrapper, name, "Z"), value
		);
	}

	/**
	 * Returns value of the double field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @return Double value of the variable.
	 */
	static inline double getDoubleField(JNIWrapper *objWrapper, const string &name) {
		return objWrapper->getEnv()->GetDoubleField(
			objWrapper->getJObject(), getFID(objWrapper, name, "D")
		);
	}

	/**
	 * Sets value of the double field inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded variable.
	 * @param name Name of the variable.
	 * @param value New value which should by which should be set this variable.
	 */
	static inline void setDoubleField(JNIWrapper *objWrapper, const string &name, double value) {
		objWrapper->getEnv()->SetDoubleField(
			objWrapper->getJObject(), getFID(objWrapper, name, "D"), value
		);
	}

	/**
	 * Returns reference to the member object inside some object.
	 *
	 * @param objWrapper Wrapper of the object with the variable with the demanded object.
	 * @param elemClass Class object of which is demanded object.
	 * @param name Name of the variable with the object.
	 * @return Reference to the member object.
	 */
	static inline jobject getObjectField(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		return env->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, JNIWrapper::getClassSignature(env, elemClass))
		);
	}

	/**
	 * Stores the reference on some object into demanded variable inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains variable to be set.
	 * @param elemClass Class object of the demanded object/variable.
	 * @param name Name of the variable with the object.
	 * @param value Reference on some object to be stored.
	 */
	static inline void setObjectField(JNIWrapper *objWrapper, jclass elemClass, const string &name, jobject value) {
		JNIEnv *env = objWrapper->getEnv();
		env->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, JNIWrapper::getClassSignature(env, elemClass)), value
		);
	}

	/**
	 * Stores the reference on some object into demanded variable inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains variable to be set.
	 * @param name Name of the variable with the object.
	 * @param value Wrapper of the object which should be set into variable.
	 */
	static inline void setObjectField(JNIWrapper *objWrapper, const string &name, JNIWrapper *value) {
		objWrapper->getEnv()->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, value->getClassSignature()), *value
		);
	}

	/**
	 * Returns reference on byte array inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded byte array.
	 * @param name Name of the variable with byte array.
	 * @return Reference on byte array inside some object.
	 */
	static inline jbyteArray getByteArray(JNIWrapper *objWrapper, const string &name) {
		return (jbyteArray)objWrapper->getEnv()->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[B")
		);
	}

	/**
	 * Stores reference on some byte array inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains variable where should stored reference.
	 * @param name Name of the variable where to store reference on byte array.
	 * @param byteArray Object of byte array of which reference should be stored into variable.
	 */
	static inline void setByteArray(JNIWrapper *objWrapper, const string &name, jbyteArray byteArray) {
		objWrapper->getEnv()->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[B"), byteArray
		);
	}

	/**
	 * Returns reference on object array inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded object array.
	 * @param elemClass Class object of the element of the array.
	 * @param name Name of the variable with object array.
	 * @return Reference on object array inside some object.
	 */
	static inline jobjectArray getObjectArray(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		return (jobjectArray)env->GetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[" + JNIWrapper::getClassSignature(env, elemClass))
		);
	}

	/**
	 * Stores reference on some object array inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains variable where should stored reference.
	 * @param elemClass Class object of the element of the array.
	 * @param name Name of the variable where to store reference on object array.
	 * @param value Object of object array of which reference should be stored into variable.
	 */
	static inline void setObjectArray(JNIWrapper *objWrapper, jclass elemClass, const string &name, jobjectArray value) {
		JNIEnv *env = objWrapper->getEnv();
		env->SetObjectField(
			objWrapper->getJObject(), getFID(objWrapper, name, "[" + JNIWrapper::getClassSignature(env, elemClass)), value
		);
	}

	/**
	 * Returns length of the array inside some object.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded array.
	 * @param elemClass Class object of the element of the array.
	 * @param name Name of the variable with array.
	 * @return Length of the array.
	 */
	static inline jsize getArrayLength(JNIWrapper *objWrapper, jclass elemClass, const string &name) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		jsize length = env->GetArrayLength(objectArray);
		env->DeleteLocalRef(objectArray);
		return length;
	}

	/**
	 * Returns reference on object inside some object array.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded object array.
	 * @param elemClass Class object of the element of the object array.
	 * @param name Name of the variable with object array.
	 * @param index Index of the object inside object array.
	 * @return Reference on object inside some object array.
	 */
	static inline jobject getObjectArrayElement(JNIWrapper *objWrapper, jclass elemClass, const string &name, jsize index) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		jobject elem = env->GetObjectArrayElement(objectArray, index);
		env->DeleteLocalRef(objectArray);
		return elem;
	}

	/**
	 * Stores reference on object inside some object array.
	 *
	 * @param objWrapper Wrapper of the object which contains demanded object array where should stored reference.
	 * @param elemClass Class object of the element of the array.
	 * @param name Name of the variable with object array.
	 * @param index Position inside object array where to store the reference.
	 * @param value Object of which reference should be stored into object array.
	 */
	static inline void setObjectArrayElement(JNIWrapper *objWrapper, jclass elemClass, const string &name, jsize index, jobject value) {
		JNIEnv *env = objWrapper->getEnv();
		jobjectArray objectArray = getObjectArray(objWrapper, elemClass, name);
		env->SetObjectArrayElement(objectArray, index, value);
		env->DeleteLocalRef(objectArray);
	}

	/**
	 * Returns class signature of the passed class.
	 *
	 * @param env Pointer to the JNI environment.
	 * @param cls Class of which signature should be returned.
	 * @return Class signature of the passed class.
	 */
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
	JNIEnv *env;		/**< Pointer to the JNI environment. */
	jobject jObject;	/**< Java object above which is created this wrapper. */
	jclass cls;			/**< Class of the object above which is created this wrapper. */
	bool localJObject;	/**< Signals if the object has been constructed locally. */
};

}

#endif

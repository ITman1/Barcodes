#include <jni.h>

namespace jni {

class JNIWrapper {
public:
	JNIWrapper(JNIEnv *env, string className, string signature = "()V", ...);
	JNIWrapper(JNIEnv *env, jobject jObject);

	inline JNIEnv* getEnv() {
		return env;
	}

	inline jobject getJObject() {
		return jObject;
	}

	inline jclass getJClass() {
		return env->GetObjectClass(jObject);
	}

	inline string getClassSignature() {
		jboolean isCopy;
		jclass cls = getJClass();

		jmethodID nameMethod = env->GetMethodID(cls, "getName", "(Ljava/lang/String;)V");
		jstring jClassName = (jstring)env->CallObjectMethod(cls, nameMethod);

		const char* strClassName = env->GetStringUTFChars(jClassName , &isCopy);
		string className = string(strClassName);
		if (isCopy == JNI_TRUE) {
			env->ReleaseStringUTFChars(jClassName , strClassName);
	    }
		return className;
	}

	inline operator jobject() {
		return jObject;
	}
protected:
	JNIEnv *env;
	jobject jObject;
};

}

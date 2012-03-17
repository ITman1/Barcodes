#ifndef JNI_JSIZE_H_
#define JNI_JSIZE_H_

#include <barlib/types.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

class jSize: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jSize(JNIEnv *env);
	jSize(JNIEnv *env, jobject jObject);
	jSize(JNIEnv *env, Size point);

	void setWidth(int width);
	void setHeight(int height);
	int getWidth();
	int getHeight();

	static jclass getJClass(JNIEnv *env);
	operator Size();
};

}

#endif

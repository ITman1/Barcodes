#ifndef JNI_JPOINT_H_
#define JNI_JPOINT_H_

#include <barlib/types.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

class jPoint: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jPoint(JNIEnv *env);
	jPoint(JNIEnv *env, jobject jObject);
	jPoint(JNIEnv *env, Point point);

	void setX(int x);
	void setY(int y);
	int getX();
	int getY();

	static jclass getJClass(JNIEnv *env);
	operator Point();
};

}

#endif

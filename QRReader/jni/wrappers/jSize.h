#include <opencv2/core/core.hpp>

#include "JNIWrapper.h"

using namespace cv;

namespace jni {

class jSize: protected JNIWrapper {
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

	operator Size();
};

}

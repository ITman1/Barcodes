#include <opencv2/core/core.hpp>

#include "JNIWrapper.h"

using namespace cv;

namespace jni {

class jPoint: protected JNIWrapper {
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

	operator Point();
};

}

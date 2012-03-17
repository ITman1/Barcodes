#ifndef JNI_JDETECTEDMARK_H_
#define JNI_JDETECTEDMARK_H_

#include <barlib/barcodes/Barcode.h>

#include "JNIWrapper.h"
#include "jPoint.h"

using namespace barcodes;

namespace jni {

class jDetectedMark: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jDetectedMark(JNIEnv *env);
	jDetectedMark(JNIEnv *env, jobject jObject);
	jDetectedMark(JNIEnv *env, DetectedMark detectedMark);

	vector<Point> getPoints();
	void setPoints(vector<Point> points);
	double getMatch();
	void setMatch(double match);
	int getFlags();
	void setFlags(int flags);

	static jclass getJClass(JNIEnv *env);
	operator DetectedMark();
};

}

#endif

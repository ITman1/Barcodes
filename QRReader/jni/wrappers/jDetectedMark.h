///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jDetectedMark.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the wrapper for accessing the members of the objects
//             of DetectedMark class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jDetectedMark.h
 *
 * @brief Defines the wrapper for accessing the members of the objects
 *        of DetectedMark class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JDETECTEDMARK_H_
#define JNI_JDETECTEDMARK_H_

#include <barlib/barcodes/DetectedMarks.h>

#include "JNIWrapper.h"
#include "jPoint.h"

using namespace barcodes;

namespace jni {

/**
 * Wraps the Java DetectedMark class.
 */
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

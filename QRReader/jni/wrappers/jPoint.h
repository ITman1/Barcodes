///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jPoint.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the wrapper for accessing the members of the objects
//             of Point class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jPoint.h
 *
 * @brief Defines the wrapper for accessing the members of the objects
 *        of Point class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JPOINT_H_
#define JNI_JPOINT_H_

#include <barlib/common.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

/**
 * Wraps the Java Point class.
 */
class jPoint: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jPoint(JNIEnv *env);
	jPoint(JNIEnv *env, jobject jObject);
	jPoint(JNIEnv *env, Point point);

	int getX();
	void setX(int x);

	int getY();
	void setY(int y);

	static jclass getJClass(JNIEnv *env);
	operator Point();
};

}

#endif

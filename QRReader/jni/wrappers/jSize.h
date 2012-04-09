///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jSize.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the wrapper for accessing the members of the objects
//             of Size class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jSize.h
 *
 * @brief Defines the wrapper for accessing the members of the objects
 *        of Size class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JSIZE_H_
#define JNI_JSIZE_H_

#include <barlib/common.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

/**
 * Wraps the Java Size class.
 */
class jSize: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jSize(JNIEnv *env);
	jSize(JNIEnv *env, jobject jObject);
	jSize(JNIEnv *env, Size point);

	int getWidth();
	void setWidth(int width);

	int getHeight();
	void setHeight(int height);

	static jclass getJClass(JNIEnv *env);
	operator Size();
};

}

#endif

///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jDataSegments.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the wrapper for accessing the members of the objects
//             of DataSegments class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jDataSegments.h
 *
 * @brief Defines the wrapper for accessing the members of the objects
 *        of DataSegments class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JDATASEGMENTS_H_
#define JNI_JDATASEGMENTS_H_

#include <barlib/barcodes/DataSegments.h>
#include <barlib/common.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

/**
 * Wraps the Java DataSegments class.
 */
class jDataSegments: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jDataSegments(JNIEnv *env);
	jDataSegments(JNIEnv *env, jobject jObject);
	jDataSegments(JNIEnv *env, DataSegments &dataSegments);

	vector<DataSegment> getSegments();
	void setSegments(vector<DataSegment> &segments);

	int getFlags();
	void setFlags(int flags);

	static jclass getJClass(JNIEnv *env);
	operator DataSegments();
};

}

#endif

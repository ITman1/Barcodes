///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// File:       jImage.h
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Defines the wrapper for accessing the members of the objects
//             of Image class in the Java.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file jImage.h
 *
 * @brief Defines the wrapper for accessing the members of the objects
 *        of Image class in the Java.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef JNI_JIMAGE_H_
#define JNI_JIMAGE_H_

#include <barlib/common/Image.h>
#include <barlib/common.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

/**
 * Wraps the Java Image class.
 */
class jImage: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jImage(JNIEnv *env);
	jImage(JNIEnv *env, jobject jObject);
	jImage(JNIEnv *env, Image image);

	Size getSize();
	void setSize(Size size);

	bool getCompressed();
	void setCompressed(bool compressed);

	int getColorFormat();
	void setColorFormat(int colorFormat);

	void getData(ByteArray &data);
	void setData(uchar *data, int length);

	static jclass getJClass(JNIEnv *env);
	operator Image();
};

}

#endif

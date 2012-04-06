#ifndef JNI_JIMAGE_H_
#define JNI_JIMAGE_H_

#include <barlib/common/Image.h>
#include <barlib/types.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

class jImage: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jImage(JNIEnv *env);
	jImage(JNIEnv *env, jobject jObject);
	jImage(JNIEnv *env, Image image);

	void setSize(Size size);
	void setCompressed(bool compressed);
	void setColorFormat(int colorFormat);
	void setData(uchar *data, int length);
	Size getSize();
	int getColorFormat();
	bool getCompressed();
	void getData(ByteArray &data);

	static jclass getJClass(JNIEnv *env);
	operator Image();
};

}

#endif

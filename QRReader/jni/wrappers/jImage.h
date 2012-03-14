#include <opencv2/core/core.hpp>
#include <barlib/barcodes/Barcode.h>

#include "JNIWrapper.h"

using namespace cv;
using namespace barcodes;

namespace jni {

class jImage: protected JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jImage(JNIEnv *env);
	jImage(JNIEnv *env, jobject jObject);
	jImage(JNIEnv *env, Image image);

	void setSize(Size size);
	void setImageFormat(int imageFormat);
	void setData(uchar *data, int length);
	Size getSize();
	int getImageFormat();
	uchar* getData();

	operator Image();
};

}

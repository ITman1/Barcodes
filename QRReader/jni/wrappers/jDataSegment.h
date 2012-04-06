#ifndef JNI_JDATASEGMENT_H_
#define JNI_JDATASEGMENT_H_

#include <barlib/barcodes/DataSegment.h>
#include <barlib/types.h>

#include "JNIWrapper.h"

using namespace barcodes;

namespace jni {

class jDataSegment: public JNIWrapper {
private:
	const static string CLASS_NAME;

public:
	jDataSegment(JNIEnv *env);
	jDataSegment(JNIEnv *env, jobject jObject);
	jDataSegment(JNIEnv *env, DataSegment &dataSegment);

	ByteArray getData();
	void setData(ByteArray &data);
	size_t getRemainderBits();
	void setRemainderBits(size_t remainderBits);
	int getFlags();
	void setFlags(int flags);
	int getMode();
	void setMode(int flags);

	static jclass getJClass(JNIEnv *env);
	operator DataSegment();
};

}

#endif

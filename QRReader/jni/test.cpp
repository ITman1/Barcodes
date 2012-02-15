#include <jni.h>
#include <vector>

extern "C" {
    JNIEXPORT jint JNICALL Java_com_qrreader_android_QrReaderAndroid_testMe(JNIEnv * env, jobject obj);
};

JNIEXPORT jint JNICALL Java_com_qrreader_android_QrReaderAndroid_testMe(JNIEnv * env, jobject obj)
{
    std::vector<int> vec;
    int i = 12;
    return i;
}
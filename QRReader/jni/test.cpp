#include <vector>
#include <iostream>

#include <jni.h>

#include <BarcodesLibrary.h>

using namespace std;

extern "C" {
    JNIEXPORT jint JNICALL Java_com_qrreader_android_QrReaderAndroid_testMe(JNIEnv * env, jobject obj);
};

JNIEXPORT jint JNICALL Java_com_qrreader_android_QrReaderAndroid_testMe(JNIEnv * env, jobject obj)
{
    int i = testMELib();
    return i;
}

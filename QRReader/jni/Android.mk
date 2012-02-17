##########( BUILDING BARCODES LIBRARY )##########
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Adding OpenCV prebuilt libraries and setting LD/INCLUDE/CFLAGS variables
include ../OpenCV/OpenCV_Android.mk

# Including OS Specifics Library
include ../GNUMake_OSSLib/include.mk

LOCAL_MODULE    := BarcodesLibrary
LOCAL_SRC_FILES := $(patsubst %,../../BarcodesLibrary/src/%,$(shell $(call oss_cat,"..\BarcodesLibrary\src_list.TXT")))
a := $(shell echo on && echo $(LOCAL_SRC_FILES) >test.out)
LOCAL_LDLIBS    += -llog -ldl -lz

include $(BUILD_SHARED_LIBRARY)

##########( BUILDING JNI LIBRARY )##########
# - availables methods for QR codes only
include $(CLEAR_VARS)

LOCAL_C_INCLUDES         += ../BarcodesLibrary/include
LOCAL_SHARED_LIBRARIES   += BarcodesLibrary
LOCAL_CFLAGS             += -fPIC

LOCAL_MODULE             := JNIBarcodesLibrary
LOCAL_SRC_FILES          := test.cpp
LOCAL_LDLIBS             += -llog -ldl -lz

include $(BUILD_SHARED_LIBRARY)
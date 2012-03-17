##########( BUILDING BARCODES LIBRARY )##########
LOCAL_PATH := $(call my-dir)

# Including OS Specifics Library
include ../GNUMake_OSSLib/include.mk

# Adding OpenCV prebuilt libraries
include ../OpenCV/OpenCV_Android.mk

include $(CLEAR_VARS)

LOCAL_C_INCLUDES         += $(OPENCV_LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES   += $(OPENCV_LOCAL_LIBRARIES)
LOCAL_CFLAGS             += $(OPENCV_LOCAL_CFLAGS)

LOCAL_MODULE             := BarcodesLibrary
LOCAL_SRC_FILES          += $(patsubst %,../../BarcodesLibrary/src/%,$(shell $(call oss_cat,"..\BarcodesLibrary\src_list.TXT")))
LOCAL_LDLIBS             += -llog -ldl -lz

include $(BUILD_STATIC_LIBRARY)

##########( BUILDING JNI LIBRARY )##########
# - availables methods for QR codes only
include $(CLEAR_VARS)

LOCAL_C_INCLUDES         += ../BarcodesLibrary/include $(OPENCV_LOCAL_C_INCLUDES)
LOCAL_STATIC_LIBRARIES   += BarcodesLibrary $(OPENCV_LOCAL_LIBRARIES) 
LOCAL_CFLAGS             += $(OPENCV_LOCAL_CFLAGS) -fPIC

LOCAL_MODULE             := JNI_QRBarcodesLibrary
LOCAL_SRC_FILES          := JNI_QRBarcodesLibrary.cpp $(foreach mod, jDetectedMark jImage jPoint jSize, wrappers/$(mod).cpp)
LOCAL_LDLIBS             += -llog -ldl -lz

include $(BUILD_SHARED_LIBRARY)
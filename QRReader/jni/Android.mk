LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include ../OpenCV/OpenCV.mk

LOCAL_MODULE    := test
LOCAL_SRC_FILES := test.cpp

include $(BUILD_SHARED_LIBRARY)

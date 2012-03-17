# Source: http://sourceforge.net/projects/opencvlibrary/files/opencv-android/2.3.1/
# In order to compile your application under cygwin
# you need to define NDK_USE_CYGPATH=1 before calling ndk-build

USER_LOCAL_PATH:=$(LOCAL_PATH)
LOCAL_PATH:=$(subst ?,,$(firstword ?$(subst \, ,$(subst /, ,$(call my-dir)))))

OPENCV_THIS_DIR:=$(patsubst $(LOCAL_PATH)\\%,%,$(patsubst $(LOCAL_PATH)/%,%,$(call my-dir)))
OPENCV_LIBS_DIR:=$(OPENCV_THIS_DIR)
OPENCV_LOCAL_C_INCLUDES:="$(LOCAL_PATH)/$(OPENCV_THIS_DIR)/include"

OPENCV_MODULES := contrib calib3d objdetect features2d video imgproc highgui ml legacy flann core
OPENCV_EXTRA_MODULES:=libjpeg libpng libtiff libjasper zlib

OPENCV_LIB_SUFFIX:=a

define add_opencv_module
	include $(CLEAR_VARS)
	LOCAL_MODULE:=opencv_$1
	LOCAL_SRC_FILES:=$(OPENCV_LIBS_DIR)/libs/Android/$(TARGET_ARCH_ABI)/libopencv_$1.$(OPENCV_LIB_SUFFIX)
	include $(PREBUILT_STATIC_LIBRARY)
endef

define add_opencv_extra_module
	include $(CLEAR_VARS)
	LOCAL_MODULE:=3rdparty_$1
	LOCAL_SRC_FILES:=$(OPENCV_LIBS_DIR)/libs/Android/$(TARGET_ARCH_ABI)/lib3rdparty_$1.$(OPENCV_LIB_SUFFIX)
	include $(PREBUILT_STATIC_LIBRARY)
endef

$(foreach module,$(OPENCV_MODULES),$(eval $(call add_opencv_module,$(module))))
$(foreach module,$(OPENCV_EXTRA_MODULES),$(eval $(call add_opencv_extra_module,$(module))))

OPENCV_LOCAL_LIBRARIES += $(foreach mod, $(OPENCV_MODULES), opencv_$(mod))
OPENCV_LOCAL_LIBRARIES += $(foreach mod, $(OPENCV_EXTRA_MODULES), 3rdparty_$(mod))

OPENCV_LOCAL_CFLAGS := -fPIC -DANDROID -fsigned-char

include $(CLEAR_VARS)

LOCAL_PATH:=$(USER_LOCAL_PATH)

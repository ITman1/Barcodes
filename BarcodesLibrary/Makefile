# Build Makefile
# File:     Makefile
# Project:  Barcodes Library
# Author:   Radim Loskot
# E-mail:   xlosko01@stud.fit.vutbr.cz

##########( Configure OpenCV include/library paths FIRST! )##########

# Tries to get OpenCV installation directory from system variable OPENCV_DIR
# Redefine this for custom installation directory or set up system variable OPENCV_DIR
OPENCV_INSTALL_DIR  := $(OPENCV_DIR)
# Include path of OpenCV
OPENCV_INCLUDE_PATH := -I$(OPENCV_INSTALL_DIR)\build\include
# Library path with compiled libraries for target platform
OPENCV_LIB_PATH     := -L$(OPENCV_INSTALL_DIR)\build\x86\mingw\lib

# Including OS Specifics Library
include ../GNUMake_OSSLib/include.mk

SRCDIR = src
OBJDIR = obj
TARGET = QRReaderCore.exe

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
INCLUDES = $(OPENCV_INCLUDE_PATH)
LIBS = $(OPENCV_LIB_PATH) -llibopencv_core231 -llibopencv_imgproc231 \
       -llibopencv_calib3d231 -llibopencv_video231 -llibopencv_features2d231 \
       -llibopencv_ml231 -llibopencv_highgui231 -llibopencv_objdetect231 \
       -llibopencv_contrib231 -llibopencv_legacy231

SRCS =   QRReaderCore.cpp
OBJS = 	 ${SRCS:%.cpp=${OBJDIR}/%.o}

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(call oss_mkdir,$(patsubst %/,%,$(dir $@))) $(OSS_RUN_QUIET)
	$(CXX) -c -o $@ $< $(INCLUDES)

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	$(call oss_mkdir,$(OBJDIR)) $(OSS_RUN_QUIET)

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean clean-all clean-outp

clean:
	$(call oss_rmdir,$(OBJDIR))

clean-outp:								# project doesnt produce any
	

clean-all: clean clean-outp
	$(call oss_rmdir,$(TARGET))


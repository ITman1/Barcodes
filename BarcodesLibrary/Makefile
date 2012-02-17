# Makefile of the Barcodes Library for most of Unix based platforms and Windows platforms
# File:     Makefile
# Project:  Barcodes Library
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

##########( Configure OpenCV installation/include/library paths FIRST! )##########

# Tries to get OpenCV installation directory from the system variable OPENCV_DIR
# Redefine this for custom installation directory or set up system variable OPENCV_DIR
OPENCV_INSTALL_DIR  := $(OPENCV_DIR)
# Include (headers) path of OpenCV
OPENCV_INCLUDE_PATH := -I$(OPENCV_INSTALL_DIR)\build\include
# Library path with compiled libraries for target platform
OPENCV_LIB_PATH     := -L$(OPENCV_INSTALL_DIR)\build\x86\mingw\lib

# Including OS Specifics Library
include ../GNUMake_OSSLib/include.mk

# Defining names of some directories, Barcodes Library and file that contains list of the sources
SRCDIR   := src
OBJDIR   := obj
LIBDIR   := $(if $(call eq,$(flavor LIBDIR),undefined),lib,$(LIBDIR))
TARGET   := $(if $(call eq,$(flavor TARGET),undefined),libBarcodesLibrary$(OSS_SHARED_LIBRARY_EXTENSION),$(TARGET))
SRC_LIST := src_list.TXT

# Flags and options for compiler
CXXFLAGS :=	$(CXXOPT) -g -Wall -fmessage-length=0
INCLUDES := $(OPENCV_INCLUDE_PATH)
LIBS     := $(OPENCV_LIB_PATH) -llibopencv_core231 -llibopencv_imgproc231 \
           -llibopencv_calib3d231 -llibopencv_video231 -llibopencv_features2d231 \
           -llibopencv_ml231 -llibopencv_highgui231 -llibopencv_objdetect231 \
           -llibopencv_contrib231 -llibopencv_legacy231

# Lists of sources and output object files
SRCS := $(shell $(call oss_cat,$(SRC_LIST)))
OBJS := ${SRCS:%.cpp=${OBJDIR}/%.o}

# Universal rule for creating object files
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(call oss_mkdir,$(patsubst %/,%,$(dir $@))) $(OSS_RUN_QUIET)
	$(CXX) -c -o $@ $< $(INCLUDES)

# Default rule
all: $(OBJDIR) $(LIBDIR)/$(TARGET)

# Creates directory for object files
$(OBJDIR):
	$(call oss_mkdir,$(OBJDIR)) $(OSS_RUN_QUIET)

# Creates final library from the object files
$(LIBDIR)/$(TARGET):	$(OBJS)
	$(call oss_mkdir,$(LIBDIR)) $(OSS_RUN_QUIET)
	$(CXX) -shared -fPIC $(OBJS) $(LIBS) -o $(LIBDIR)/$(TARGET)

.PHONY: clean clean-all debug release tests

# Debug
debug:
	$(MAKE) -B all CXXOPT="-g3 -D debugging"

# Release
release:
	$(MAKE) -B all CXXOPT="-O3 -D releasing"

# Recursively calls build Makefile of the tests
tests:
	$(MAKE) -C tests

# Cleans directory for object files
clean:
	$(call oss_rmdir,$(OBJDIR))

# Cleans everything generated
clean-all: clean clean-outp
	$(call oss_rmdir,$(LIBDIR))


# Makefile which builds test executable programs which uses Barcodes Library
# These programs are called by RunTests.py
# File:     Makefile
# Project:  Barcodes Library
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

# Including OS Specifics Library
include ../../GNUMake_OSSLib/include.mk

# Defining names of some directories and Barcodes Library
SRCDIR         := src
TESTSDIR       := tests
BARCODES_LIB   := libBarcodesLibrary$(OSS_SHARED_LIBRARY_EXTENSION)

# Flags and options for compiler
CXXFLAGS :=	$(CXXOPT) -g -Wall -fmessage-length=0
INCLUDES := -I../include
LIBS     := -L./$(TESTSDIR) -lBarcodesLibrary

# Lists of sources and output executable programs
SRCS     := $(shell $(call oss_ls,$(SRCDIR)))
EXECS    := $(patsubst %,$(TESTSDIR)/%,$(patsubst %.cpp,%$(OSS_EXECUTABLE_EXTENSION),$(SRCS)))

# Universal rule for compiling/linking executable program
$(TESTSDIR)/%$(OSS_EXECUTABLE_EXTENSION) : $(SRCDIR)/%.cpp
	$(CXX) -o $@ $< $(INCLUDES) $(LIBS)

# Main entrance rule
all: clean-builds $(TESTSDIR) $(BARCODES_LIB) $(EXECS)

# Cleans an old compilation of the library
clean-builds:
	$(MAKE) clean
	$(MAKE) -C ../ clean

# Compiles a library again and stores it to "tests" directory
$(BARCODES_LIB):
	$(MAKE) -C ../ -B all TARGET="$(BARCODES_LIB)" LIBDIR="tests/$(TESTSDIR)"

# Creates "tests" directory
$(TESTSDIR):
	$(call oss_mkdir,$(TESTSDIR)) $(OSS_RUN_QUIET)

.PHONY: clean debug release run

# Debug
debug:
	$(MAKE) -B all CXXOPT="-g3 -D debugging"

# Release
release:
	$(MAKE) -B all CXXOPT="-O3 -D releasing"

# Deletes "tests" directory
clean:
	$(call oss_rmdir,$(TESTSDIR))


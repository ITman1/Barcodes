# OS specific variables
# File:     os_vars.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01@stud.fit.vutbr.cz

ifndef _OS_VARS_MK
_OS_VARS_MK = _OS_VARS_MK

# Set OS name to the variable OSS_OS_NAME
ifeq ($(OS),Windows_NT)
    OSS_OS_NAME := windows
else
    UNAME := $(shell uname -s)
    ifneq ($(findstring Linux,$(UNAME)),)
        OSS_OS_NAME := linux
    endif
    ifneq ($(findstring CYGWIN,$(UNAME)),)
        OSS_OS_NAME := windows
    endif
endif

# Location of the NUL device
ifeq ($(OSS_OS_NAME), windows)
	OSS_DEVNULL := NUL
else
	OSS_DEVNULL := /dev/null
endif

endif
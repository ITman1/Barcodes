# I/O macros
# File:     io.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

ifndef _IO_MK
_IO_MK = _IO_MK

_IO_MK_RUN_DIR = $(dir $(lastword $(MAKEFILE_LIST)))
include $(_IO_MK_RUN_DIR)os_vars.mk

# Pipes stdout and stderr to the null device (no output is printed)
OSS_RUN_QUIET := 1>$(OSS_DEVNULL) 2>&1

# Concatenates files or prints on stdout
# @param $1 filenames of the files
ifeq ($(OSS_OS_NAME), windows)
    oss_cat = type $1
else
    oss_cat = cat $1
endif

# Prints text on stdout
# @param $1 text to be printed
ifeq ($(OSS_OS_NAME), windows)
    oss_echo = ECHO $1
else
    oss_echo = echo $1
endif

endif
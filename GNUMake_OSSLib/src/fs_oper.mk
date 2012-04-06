# Filesystem operations
# File:     fs_oper.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

ifndef _FS_OPER_MK
_FS_OPER_MK = _FS_OPER_MK

_FS_OPER_MK_RUN_DIR = $(dir $(lastword $(MAKEFILE_LIST)))
include $(_FS_OPER_MK_RUN_DIR)os_vars.mk
include $(_FS_OPER_MK_RUN_DIR)common.mk

# Tests whether file/dir exists
# @param $1 filename of the file/dir
# @param $2 adds negate to the condition "not" or ""
# @param $3 specifies type of the file (regular file/directory)
ifeq ($(OSS_OS_NAME), windows)
    oss_if_exist = IF $(if $(call eq,$2,not), NOT) EXIST $1 $(if $(call eq,$3,dir), /)
else
    oss_if_exist = [ $(if $(call eq,$3,dir), -d, -f) $1 ] && $(if $(call eq,$2,not), : ||)
endif

# Creates folder
# @param $1 filename of the folder
ifeq ($(OSS_OS_NAME), windows)
    oss_mkdir = IF NOT EXIST "$(patsubst %/, %, $1)/" MKDIR $(subst /,\,$(patsubst %/, %, $1))
else
    oss_mkdir = mkdir -p $1
endif

# Removes file from the filesystem
# @param $1 filename to the file
ifeq ($(OSS_OS_NAME), windows)
    oss_rm = IF EXIST "$(patsubst %/, %, $1)" DEL /F /S /Q "$(patsubst %/, %, $1)"
else
    oss_rm = rm -f $1
endif

# Removes folder from the filesystem
# @param $1 filename to the folder
ifeq ($(OSS_OS_NAME), windows)
    oss_rmdir = IF EXIST "$(patsubst %/, %, $1)/" RD /S /Q "$(patsubst %/, %, $1)"
else
    oss_rmdir = rm -rf $1
endif

# Copies file from one location to the another one
# @param $1 filepath to the source location
# @param $2 filepath to the destination location
# @param $3 file filter
ifeq ($(OSS_OS_NAME), windows)
    oss_cp = -ROBOCOPY $(subst /,\, "$1" "$2" $(if $(call eq,$(origin 3),undefined),"*","$3")) /E
else
    oss_cp = cp -f $1 $2
endif

# Prints directory file list on stdout
# @param $1 directory with files which should be printed
ifeq ($(OSS_OS_NAME), windows)
    oss_ls = dir /B $1
else
    oss_ls = ls $1
endif

endif
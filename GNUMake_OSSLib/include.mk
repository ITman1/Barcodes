# Includes all library files
# File:     proc.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

_RUN_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(_RUN_DIR)src/fs_oper.mk
include $(_RUN_DIR)src/proc.mk
include $(_RUN_DIR)src/io.mk
include $(_RUN_DIR)src/common.mk

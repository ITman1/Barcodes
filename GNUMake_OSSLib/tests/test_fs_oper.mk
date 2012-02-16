# Test Makefile - Filesystem operations (called by test_fs_oper.py)
# File:     test_fs_oper.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

include ../include.mk

.PHONY: create-tmp cp-tmp del-file del-tmp

create-tmp:
	$(call oss_mkdir,tmp)

cp-tmp:
	$(call oss_cp,tmp,tmp2)

del-file:
	$(call oss_rm,tmp/file)

del-tmp:
	$(call oss_rmdir,tmp)
	
ls-tmp:
	$(call oss_ls,tmp) > tmp/ls.out

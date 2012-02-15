# Test script - Filesystem operations (called by test.py)
# File:     fs_oper.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01@stud.fit.vutbr.cz

import os
import shutil
import sys

MAKE_CMD = sys.argv[1] if len(sys.argv) > 1 else "make"

def create_tmp():
    f = open("tmp2/file2", 'w')
    f.write("test2")

def create_tmp_file():
    if not os.path.exists("tmp"):
        os.mkdir("tmp")
    f = open("tmp/file", 'w')
    f.write("test")

def destroy_env():
    if os.path.exists("tmp"):
        shutil.rmtree("tmp")
    if os.path.exists("tmp2"):
        shutil.rmtree("tmp2")

assert(os.system(MAKE_CMD + " -f test_fs_oper.mk create-tmp") == 0 and os.path.exists("tmp"))
assert(os.system(MAKE_CMD + " -f test_fs_oper.mk cp-tmp") == 0 and os.path.exists("tmp2"))
assert(os.system(MAKE_CMD + " -f test_fs_oper.mk del-tmp") == 0 and not os.path.exists("tmp"))
shutil.rmtree("tmp2")

create_tmp_file()
assert(os.system(MAKE_CMD + " -f test_fs_oper.mk cp-tmp") == 0 and os.path.exists("tmp2"))

create_tmp_file()
assert(os.system(MAKE_CMD + " -f test_fs_oper.mk del-tmp") == 0 and not os.path.exists("tmp"))
destroy_env()
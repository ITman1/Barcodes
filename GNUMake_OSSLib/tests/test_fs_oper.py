# Test script - Filesystem operations (called by test.py)
# File:     test_fs_oper.py
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

import os
import shutil
import sys
import filecmp
import re

# Getting make cmd which should be passed via run arguments
MAKE_CMD = (sys.argv[1] if len(sys.argv) > 1 else "make") + " -f test_fs_oper.mk "

# Creates "tmp" directory and file "file" inside this directory 
def create_tmp_file():
    if not os.path.exists("tmp"):
        os.mkdir("tmp")
    f = open("tmp/file", 'w')
    f.write("test")

# Removes "tmp" and "tmp2" directories
def destroy_env():
    if os.path.exists("tmp"):
        shutil.rmtree("tmp")
    if os.path.exists("tmp2"):
        shutil.rmtree("tmp2")

# Replaces string inside specified file for a new one
# @param file filename of the file where to replace text strings
# @param _from string to be replaced
# @param _to new string
def file_replace(file, _from, _to):
    content = open(file).read()
    out = open(file, "w")
    out.write(re.sub(_from, _to, content))
    out.close()
    return True

try:

    # TEST - creating tmp directory
    assert(os.system(MAKE_CMD + "create-tmp") == 0)
    assert(os.path.exists("tmp"))
    
    # TEST - copying tmp directory to tmp2 directory
    assert(os.system(MAKE_CMD + "cp-tmp") == 0) 
    assert(os.path.exists("tmp2"))
    
    # TEST - deleting tmp directory
    assert(os.system(MAKE_CMD + "del-tmp") == 0)
    assert(not os.path.exists("tmp"))
    shutil.rmtree("tmp2")
    
    # TEST - copying tmp directory to tmp2 directory with the file inside
    create_tmp_file()
    assert(os.system(MAKE_CMD + "cp-tmp") == 0) 
    assert(os.path.exists("tmp2"))
    
    # TEST - deleting tmp directory with the file inside
    create_tmp_file()
    assert(os.system(MAKE_CMD + "del-tmp") == 0) 
    assert(not os.path.exists("tmp"))
    
    # TEST - testing file list output - ls or dir output
    create_tmp_file()
    assert(os.system(MAKE_CMD + "ls-tmp") == 0)
    assert(file_replace("tmp/ls.out", "\r", "")) 
    assert(file_replace("tmp/ls.out", "\n", "")) 
    assert(filecmp.cmp("tmp/ls.out", "refs/test_fs_oper_ls.out"))
except SomeError:
    sys.stderr.write("EXCEPTION CAUGHT !!!") 
finally:
    # removing rubbish
    destroy_env()
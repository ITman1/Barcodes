# Calls test Makefiles and Python scripts which tests results
# File:     test.py
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

import os

MAKE_CMD = "make"

# Calling test of the filesystem operations
os.system("python test_fs_oper.py " + MAKE_CMD)
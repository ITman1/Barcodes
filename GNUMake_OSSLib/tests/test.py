# Calls test Makefiles and Python scripts which tests results
# File:     test.py
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01@stud.fit.vutbr.cz

import os

MAKE_CMD = "make"

os.system("python test_fs_oper.py " + MAKE_CMD)
# Script that run executable programs which test library
# File:     RunTests.py
# Project:  Barcodes Library
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

import os
import sys
from subprocess import call

fileList = []
rootdir = "tests"

for root, subFolders, files in os.walk(rootdir):
    for file in files:
        fileList.append(os.path.join(root,file))

if (len(sys.argv) > 1):
    fileList = [sys.argv[1]]

for file in fileList:
    if os.path.splitext(file)[1] == '.exe':
        print('========== ' + file + ' ==========')
        call([file])





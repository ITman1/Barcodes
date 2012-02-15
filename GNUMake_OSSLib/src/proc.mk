# Process macros
# File:     proc.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01@stud.fit.vutbr.cz

ifndef _PROC_MK
_PROC_MK = _PROC_MK

# Retrieves return code of the last process
ifeq ($(OSS_OS_NAME), windows)
    OSS_RETCODE = %ERRORLEVEL%
else
    OSS_RETCODE = $?
endif

# Kills process by PID
# @param $1 PID of the process to be killed
ifeq ($(OSS_OS_NAME), windows)
    oss_kill_pid = TASKKILL /PID $1
else
    oss_kill_pid = kill -9 $1
endif

# Kills process
# @param $1 name of the process to be killed
ifeq ($(OSS_OS_NAME), windows)
    oss_kill_name = TASKKILL /IM $1
else
    oss_kill_name = killall -s 9 $1
endif

endif
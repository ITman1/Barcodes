# Contains some useful GNU Make extension macros
# File:     common.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

ifndef _COMMON_MK
_COMMON_MK = _COMMON_MK

# Test whether two strings equals
eq = $(and $(findstring $1,$2),$(findstring $2,$1))

endif
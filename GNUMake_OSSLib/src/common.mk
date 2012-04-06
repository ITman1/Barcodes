# Contains some useful GNU Make extension macros
# File:     common.mk
# Project:  GNU Make OS Specifics
# Author:   Radim Loskot
# E-mail:   xlosko01(at)stud.fit.vutbr.cz

ifndef _COMMON_MK
_COMMON_MK = _COMMON_MK

# Test whether two strings equals
eq = $(and $(findstring $1,$2),$(findstring $2,$1))

# Credits: John Graham
# http://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

endif
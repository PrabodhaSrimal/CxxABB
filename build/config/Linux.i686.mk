####################################################################################################
#                                                                                                  #
#                                                          _|        _|                            #
#                    _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|                        #
#                  _|          _|_|      _|_|    _|    _|  _|    _|  _|    _|                      #
#                  _|        _|    _|  _|    _|  _|    _|  _|    _|  _|    _|                      #
#                    _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|                        #
#                                                                                                  #
#                           CxxABB - C++ Application Building Blocks                               # 
#                                                                                                  #
#                  Copyright (C) 2017 Prabodha Srimal <prabodha007@gmail.com>                      #
#                                                                                                  #
####################################################################################################

# @version $Revision: 24 $:
# @author  $Author: prabodar $:
# @date    $Date: 2012-11-22 14:47:10 +0530 (Thu, 22 Nov 2012) $:
# @id      $Id: Linux.i686.mk 24 2012-11-22 09:17:10Z prabodar $:

## System OS and Arch specific Makefiles for Linux i386/i486/i686 systems

## Tools
CC		= gcc
CXX		= g++
LINK	= $(CXX)
LIB		= ar -crs
RANLIB  = ranlib
SHLIB   = $(CXX) -shared -Wl,-soname,$(notdir $@) -o $@
#SHLIBLN = $(POCO_BASE)/build/script/shlibln
STRIP   = strip
MAKE.DEP= $(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$(OBJ.DIR)/%.o,$<)' $< -MF $@

SHELL	= bash
RM		= rm -rf
COPY	= cp -p
MOVE	= mv -f
MKDIR	= mkdir -p
TESTDIR	= test -d
ECHO	= echo

SHARED.LIB.PREFIX	= lib
SHARED.LIB.EXT		= .so
STATIC.LIB.PREFIX	= lib
STATIC.LIB.EXT		= .a

## COMPILER OPTIONS

## http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
## http://wiki.linuxquestions.org/wiki/Library

OPT.WARN.STOP 				= -W #Inhibit all warning messages.
OPT.WARN.LEVEL 				= -Wall
OPT.WARN.ERROR 				= -Werror #Make all warnings into errors.
OPT.WARN.TYPE 				= -Wimplicit-int
OPT.WARN.FORMAT 			= -Wformat
OPT.WARN.CONVR 				= -Wconversion
OPT.WARN.SHADOW 			= -Wshadow
OPT.WARN.POINTERARITH 		= -Wpointer-arith
OPT.WARN.CAST 				= -Wcast-qual -Wcast-align
OPT.WARN.CHAR				= -Wchar-subscripts
OPT.WARN.STRING 			= -Wwrite-strings
OPT.WARN.ISOCOMPLP 			= #-pedantic #-pedantic-errors
OPT.WARN.COMPARE			= -Wsign-compare

## COMPILER FLAGS
OPT.FLAG.COMMON 			= -fno-common
OPT.FLAG.ANSI 				= -ansi
OPT.FLAG.ENUM 				= -fshort-enums

OPT.CPU.ARCH				= -m32 
OPT.CPU.TUNE 				= -march=i686
OPT.CXX.VER					= #-std=c++0x

STATICOPT_CC    =
STATICOPT_CXX   =
STATICOPT_LINK  = -static

SHAREDOPT_CC    = -fPIC
SHAREDOPT_CXX   = -fPIC
SHAREDOPT_LINK  = -shared -Wl,--no-undefined 
#-Wl,-soname,libCxxAbbCore.so
#-Wl,-rpath,$(LINKPATH)

DEBUGOPT_CC     = -g -DNDEBUG
DEBUGOPT_CXX    = -g -DNDEBUG
DEBUGOPT_LINK   = -g

# -O -O1 -O2 -O3 Full optimization.
RELEASEOPT_CC   = -O -O3 -flto
RELEASEOPT_CXX  = -O -O3 -flto
RELEASEOPT_LINK = -O -O3 -flto

#-D_XOPEN_SOURCE=500
SYS.FLAGS	= -D_GNU_SOURCE -D_REENTRANT -D_THREAD_SAFE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE\
 -DLINUX -DINTEL -DS_DEF_AAA -DS_DEF_DELETE
SYS.LIBS	= -lpthread -ldl -lrt

## this is optional, can be detected by code
#CXXABB.FLAGS += -DCXXABB_OS_FAMILY=0x01 -DCXXABB_ARCH=0x01

ifeq ($(COMPILE.TYPE), D)
	OPT.DEBUG 		= $(DEBUGOPT_CXX)
	LINKER_FLAGS	= $(DEBUGOPT_LINK)
endif
ifeq ($(COMPILE.TYPE), R)
	OPT.DEBUG  		= $(RELEASEOPT_CXX)
	LINKER_FLAGS	= $(RELEASEOPT_LINK)
endif

TARGET.ARCH.SUFFIX = -x86

ifeq ($(TARGET.TYPE), DYLIB)
	SYS.FLAGS += $(SHAREDOPT_CXX)
	LINKER_FLAGS += $(SHAREDOPT_LINK)
	TARGET.PREFIX = lib
	TARGET.SUFFIX = .so
endif

ifeq ($(TARGET.TYPE), STLIB)
	SYS.FLAGS += $(STATICOPT_CXX)
	LINKER_FLAGS += $(STATICOPT_LINK)
	TARGET.PREFIX = lib
	TARGET.SUFFIX = .a
endif

ifeq ($(TARGET.TYPE), EXE)
	TARGET.PREFIX =
	TARGET.SUFFIX = 
endif



OPT.WARN = $(OPT.WARN.LEVEL) $(OPT.WARN.STOP) $(OPT.WARN.POINTERARITH) $(OPT.WARN.CAST) \
$(OPT.WARN.STRING) $(OPT.WARN.FORMAT) $(OPT.WARN.SHADOW) $(OPT.WARN.ISOCOMPLP) $(OPT.WARN.COMPARE)
OPT.FLAG = $(OPT.FLAG.COMMON) $(OPT.FLAG.ANSI) $(OPT.FLAG.ENUM)
OPT.MISC = $(OPT.CPU.ARCH) $(OPT.CPU.TUNE) $(OPT.DEBUG) $(OPT.CXX.VER)

INCLUDES += $(POSIX.INCLUDES)
#HEADER += $(POSIX.HEADER)
SOURCE += $(POSIX.SOURCE)
LINKPATH += $(POSIX.LINKPATH)
LIBS += $(POSIX.LIBS) $(SYS.LIBS)

## Compiler and Linker Flags

CFLAGS          =
CXXFLAGS        = $(SYS.FLAGS) $(OPT.WARN) $(OPT.FLAG) $(OPT.MISC) $(CXXABB.FLAGS) $(INCLUDES)
LINKFLAGS       = $(LINKER_FLAGS) $(LINKPATH)

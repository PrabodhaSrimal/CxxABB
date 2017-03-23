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

# @version $Revision: 26 $:
# @author  $Author: prabodar $:
# @date    $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $:
# @id      $Id: Makefile 26 2017-02-07 18:30:33Z prabodar $:

## CxxAbbCore Module - Main Makefile

SUBDIR = 

HEADER = 
		 
SOURCE = Core.cpp 
SOURCE += Platform.cpp 
SOURCE += Debug.cpp 
SOURCE += Exception.cpp 
SOURCE += ExceptionHandler.cpp
SOURCE += Timestamp.cpp 
SOURCE += TimeZone.cpp 
SOURCE += DateTime.cpp 
SOURCE += LocalDateTime.cpp 
SOURCE += MemoryPool.cpp
SOURCE += Sys/Atomicity.cpp
SOURCE += Sys/Mutex.cpp 
SOURCE += Sys/SigEvent.cpp
SOURCE += Sys/WaitCondition.cpp
SOURCE += Sys/Thread.cpp
SOURCE += Sys/SignalToException.cpp
SOURCE += Sys/Environment.cpp

POSIX.HEADER = 

POSIX.SOURCE = Sys/posix/MutexImpl.cpp 
POSIX.SOURCE += Sys/posix/SigEventImpl.cpp 
POSIX.SOURCE += Sys/posix/ThreadImpl.cpp
POSIX.SOURCE += Sys/posix/EnvironmentImpl.cpp

WIN32.HEADER = 
WIN32.SOURCE = Sys/win32/Atomicity.win.cpp 

TEST.HEADER = 

TEST.SOURCE = CoreTest.cpp 
TEST.SOURCE += AutoPtrTest.cpp 
TEST.SOURCE += ScopedPtrTest.cpp 
TEST.SOURCE += SharedPtrTest.cpp
TEST.SOURCE += RefCountedObjTest.cpp 
TEST.SOURCE += BufferTest.cpp 
TEST.SOURCE += MemoryPoolTest.cpp
TEST.SOURCE += DateTimeTest.cpp 
TEST.SOURCE += LocalDateTimeTest.cpp
TEST.SOURCE += ThreadTest.cpp 
TEST.SOURCE += EnvironmentTest.cpp 

#SOURCE := $(wildcard src/*.cpp) $(foreach sdir,$(SUBDIR),$(wildcard src/$(sdir)/*.cpp))

TARGET.NAME = CxxAbbCore
TARGET.TYPE = STLIB
#TARGET.TYPE = DYLIB

INCLUDES = -I./include
POSIX.INCLUDES = -Isrc/posix -Isrc/Sys/posix
WIN32.INCLUDES =

TEST.INCLUDES = -I./include

INCLUDE.MODULES = #CxxAbbCore test

LINKPATH = #-L../lib
POSIX.LINKPATH =
WIN32.LINKPATH =

# set only external linkpaths
TEST.LINKPATH = 

LIBS = #-lCxxAbbCore -lCxxAbbUtil
POSIX.LIBS =
WIN32.LIBS =

TEST.LIBS = -lCxxAbbCore

## Do not change this include
include $(BLD.PATH)/Common.mk

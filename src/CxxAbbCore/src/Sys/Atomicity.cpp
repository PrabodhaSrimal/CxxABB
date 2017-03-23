/**
 *                                                             _|        _|
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|
 *                     _|          _|_|      _|_|    _|    _|  _|    _|  _|    _|
 *                     _|        _|    _|  _|    _|  _|    _|  _|    _|  _|    _|
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|
 *
 *                              CxxABB - C++ Application Building Blocks
 *
 *                     Copyright (C) 2017 Prabodha Srimal <prabodha007@gmail.com>
 *
 *
 * Atomicity.cpp
 *
 * FileId      : $Id: Atomicity.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Atomic Operations
 *
 */

#include <CxxAbb/Sys/Atomicity.h>

/// Select poper implementation based on Build Flags

#ifdef CXXABB_DEF_ATOMICITY_GCC_X86

#include "posix/Atomicity.gcc.x86.cpp"

#elif defined(CXXABB_DEF_ATOMICITY_GCC_X86_64)

#include "posix/Atomicity.gcc.x86_64.cpp"

#elif defined(CXXABB_DEF_ATOMICITY_WINDOWS)

#include "win32/Atomicity.win.cpp"

#elif defined(CXXABB_DEF_ATOMICITY_PTHREAD)

#include "Atomicity.pthread.cpp"

#elif CXXABB_OS == CXXABB_OS_LINUX

#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1) || __GNUC__ > 4)

#include "posix/Atomicity.gcc.cpp"

#elif defined(__GNUC__)

#if (CXXABB_ARCH == CXXABB_ARCH_IA32)

#include "posix/Atomicity.gcc.x86.cpp"

#elif (CXXABB_ARCH == CXXABB_ARCH_IA64) || (CXXABB_ARCH == CXXABB_ARCH_AMD64)

#include "posix/Atomicity.gcc.x86_64.cpp"

#endif

#endif

#elif CXXABB_OS == CXXABB_OS_WINDOWS

#include "win32/Atomicity.win.cpp"

#else

#include "Atomicity.genaric.cpp"

#endif






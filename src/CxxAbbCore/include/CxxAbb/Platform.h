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
 * Platform.h
 *
 * FileId      : $Id: Platform.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 11, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Global platform identifications and definitions
 *
 */

#ifndef CXXABB_CORE_PLATFORM_H_
#define CXXABB_CORE_PLATFORM_H_

/// Compiler macro guide - http://predef.sourceforge.net/

/// Platform Ids
#define CXXABB_OS_LINUX				0x01
#define CXXABB_OS_WINDOWS			0x02
#define CXXABB_OS_CYGWIN			0x03
#define CXXABB_OS_BSD				0x04
#define CXXABB_OS_SOLARIS			0x05

#define CXXABB_OS_FAMILY_UNIX		0x01
#define CXXABB_OS_FAMILY_WINDOWS	0x02
#define CXXABB_OS_FAMILY_BSD		0x03

#define CXXABB_COMPILER_GCC			0x01
#define CXXABB_COMPILER_CLANG		0x02
#define CXXABB_COMPILER_MSC			0x03
#define CXXABB_COMPILER_MINGW		0x04

/// Build version to 0xMJMNPPPP
#define _VERSION_BUILDER(mj,mn,ph) (((mj) << 24) + ((mn) << 16) + (ph))

#ifndef CXXABB_COMPILER

#if defined(_MSC_VER)

#define CXXABB_COMPILER			CXXABB_COMPILER_MSC
#define CXXABB_COMPILER_VERSION _MSC_FULL_VER

#elif defined(__MINGW32__) || defined(__MINGW64__)

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <windef.h>

#define CXXABB_COMPILER			CXXABB_COMPILER_MINGW
#if defined(__MINGW32__)
#define CXXABB_COMPILER_VERSION _VERSION_BUILDER(__MINGW32_MAJOR_VERSION,__MINGW32_MINOR_VERSION,0)
#else
#define CXXABB_COMPILER_VERSION _VERSION_BUILDER(__MINGW64_MAJOR_VERSION,__MINGW64_MINOR_VERSION,0)
#endif

#elif defined(__clang__)

#define CXXABB_COMPILER			CXXABB_COMPILER_CLANG
#define CXXABB_COMPILER_VERSION _VERSION_BUILDER(__clang_major__, __clang_minor__, \
	__clang_patchlevel__)

#elif defined(__GNUC__)

#define CXXABB_COMPILER			CXXABB_COMPILER_GCC
#define CXXABB_COMPILER_VERSION _VERSION_BUILDER(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)

#endif

#endif // CXXABB_COMPILER

#ifndef CXXABB_OS_FAMILY

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__) \
	defined(__TOS_LINUX__)
	#define CXXABB_OS_FAMILY	CXXABB_OS_FAMILY_UNIX
	#define CXXABB_OS			CXXABB_OS_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined( _MSC_VER)
	#define CXXABB_OS_FAMILY	CXXABB_OS_FAMILY_WINDOWS
	#define CXXABB_OS			CXXABB_OS_WINDOWS
#elif defined(__CYGWIN__)
	#define CXXABB_OS_FAMILY	CXXABB_OS_FAMILY_UNIX
	#define CXXABB_OS			CXXABB_OS_CYGWIN
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	#define CXXABB_OS_FAMILY	CXXABB_OS_FAMILY_UNIX | CXXABB_OS_FAMILY_BSD
	#define CXXABB_OS			CXXABB_OS_BSD
#elif defined(sun) || defined(__sun)
	#define CXXABB_OS_FAMILY	CXXABB_OS_FAMILY_UNIX
	#define CXXABB_OS			CXXABB_OS_SOLARIS
#endif

#endif

/// Architecture

#define CXXABB_ARCH_IA32			0x01
#define CXXABB_ARCH_IA64			0x02
#define CXXABB_ARCH_AMD64			0x03

#ifndef CXXABB_ARCH

#if defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
	#define CXXABB_ARCH			CXXABB_ARCH_IA32
	#define CXXABB_ARCH_LITTLE_ENDIAN	1
#elif defined(_IA64) || defined(__IA64__) || defined(__ia64__) || defined(__ia64) || \
	defined(_M_IA64)
	#define CXXABB_ARCH			CXXABB_ARCH_IA64
	#if defined(hpux) || defined(_hpux)
		#define CXXABB_ARCH_BIG_ENDIAN		1
	#elif
		#define CXXABB_ARCH_LITTLE_ENDIAN	1
	#endif
#elif defined(__x86_64__) || defined(_M_X64) || defined(__amd64__) || defined(_M_AMD64)
	#define CXXABB_ARCH			CXXABB_ARCH_AMD64
	#define CXXABB_ARCH_LITTLE_ENDIAN	1
#endif

#endif

/// Specialized includes
#if CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_UNIX
#include "CxxAbb/Platform_POSIX.h"
#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_WINDOWS
#include "CxxAbb/Platform_WIN32.h"
#endif

namespace CxxAbb
{

const char * PlatformOsName(int _iOsId = CXXABB_OS_FAMILY);

const char * PlatformOsFamilyName(int _iOsFamilyid = CXXABB_OS_FAMILY);

const char * PlatformArchName(int _iArchId = CXXABB_ARCH);

bool IsPlatformLittleEndian();

} /* namespace CxxAbb */

#endif /* CXXABB_CORE_PLATFORM_H_ */

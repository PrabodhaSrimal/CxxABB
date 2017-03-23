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
 * Core.h
 *
 * FileId      : $Id: Core.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 11, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : This file should be the first include of Core library modules
 *
 */

#ifndef CXXABB_CORE_CORE_H_
#define CXXABB_CORE_CORE_H_


#include <CxxAbb/Config.h>
#include <CxxAbb/Version.h>

/// Library export/import definitions

#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32)
    #define CXXABB_EXPORT __declspec(dllexport)
    #define CXXABB_IMPORT __declspec(dllimport)
#elif __GNUC__ >= 4
    #define CXXABB_EXPORT __attribute__((visibility("default")))
    #define CXXABB_IMPORT
#else
    #define CXXABB_EXPORT
    #define CXXABB_IMPORT
#endif

#if defined(CXXABB_API_EXPORT)
    #define CXXABB_API CXXABB_EXPORT
#else
    #define CXXABB_API CXXABB_IMPORT
#endif


/// General Definitions


/// macro to join the two args X and Y; even they themselves are macros
#define CXXABB_JOIN(X, Y)		CXXABB_DO_JOIN(X, Y)
#define CXXABB_DO_JOIN(X, Y)	CXXABB_DO_JOIN2(X, Y)
#define CXXABB_DO_JOIN2(X, Y)	X##Y

#define CXXABB_DO_TOSTRING(s)	#s
#define CXXABB_TOSTRING(s)		CXXABB_DO_TOSTRING(s)

/**
 * @namespace CxxAbb
 * @brief CxxAbb Core Library module
 *
 * The core library module dose not have any dependencies with any other CxxAbb modules.
 * It Uses C++ StdLib facilities and extends them to use conveniently.
 */
namespace CxxAbb
{
	class CXXABB_API Debugger;
	class CXXABB_API SourceLineInfo;
	class CXXABB_API AtomicCounter;
	class CXXABB_API AtomicRefCounted;
	class CXXABB_API ByteOrder;
	template <typename T> class CXXABB_API TypeInfo;
	template <class Obj> class CXXABB_API AutoPtr;
	template <class Obj> class CXXABB_API ScopedPtr;
	template <class Obj> class CXXABB_API ScopedArrayPtr;
	template <class Obj> class CXXABB_API SharedPtr;
	template <class Obj> class CXXABB_API SharedArrayPtr;
	template <class Obj, class ReleasePolicy, class OwnershipPolicy> class CXXABB_API SmartPtr;

	namespace Sys
	{
		class CXXABB_API Mutex;
		class CXXABB_API FastMutex;
		template <class MutexClass> class CXXABB_API ScopedLock;
		template <class MutexClass> class CXXABB_API ScopedUnlock;
		class CXXABB_API Thread;
		class CXXABB_API ThreadErrorHandler;
		class CXXABB_API SignalToException;
		class CXXABB_API Environment;
	}

//TODO: Core classes goes here
}

/// default includes
#include <cstdlib>
#include <cstdio>
#include <cstddef>

//TODO: identify default includes
//cstdarg, stdarg.h, , sys/resource.h, ctime, sys/types.h, signal.h

#include <CxxAbb/Platform.h>
#include <CxxAbb/Types.h>
#include <CxxAbb/Exception.h>

#endif /* CXXABB_CORE_CORE_H_ */

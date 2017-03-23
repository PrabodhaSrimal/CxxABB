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
 * Debug.h
 *
 * FileId      : $Id: Debug.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 13, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Debugging facilities - Macros and helper functions
 *
 */

#ifndef CXXABB_CORE_DEBUG_H_
#define CXXABB_CORE_DEBUG_H_

#include <CxxAbb/Core.h>

#ifdef CXXABB_DEBUG
#include <iostream>
#endif

namespace CxxAbb
{

class CXXABB_API Debugger
{
public:
	static void Assert(const char* _zCond, const char* _zFile, int _iLine);
	static void NullPointer(const char* _zPtr, const char* _zFile, int _iLine);
	static void Debug(const char* _zMsg, const char* _zFile, int _iLine);
	static void DebugBreak(const char* _zMsg, const char* _zFile, int _iLine);
protected:
	static std::string what(const char* _zMsg, const char* _zFile, int _iLine);
};

} /* namespace CxxAbb */

#ifdef CXXABB_DEBUG

	#define DEBUGLOG(msg) \
		CxxAbb::Debugger::Debug(msg, __FILE__, __LINE__);
	#define DEBUGBREAK(msg) \
		CxxAbb::Debugger::DebugBreak(msg, __FILE__, __LINE__);

#else

	#define DEBUGLOG(msg)
	#define DEBUGBREAK(msg)

#endif


#ifdef CXXABB_DEBUG
	#define ASSERT(cond) \
		if(!(cond)) { CxxAbb::Debugger::Assert(#cond, __FILE__, __LINE__); } else (void) 0;
#else
	#define ASSERT(cond)
#endif

#ifdef CXXABB_DEBUG
	#define CHECKNULL(ptr) \
		if (!(ptr)) CxxAbb::Debugger::NullPointer(#ptr, __FILE__, __LINE__);  else (void) 0;
#else
	#define CHECKNULL(ptr)
#endif

/// Compile time asserts
/// based on Boost static_assert s

template <bool x>
struct CXXABB_STATIC_ASSERTION_FAILURE;


template <>
struct CXXABB_STATIC_ASSERTION_FAILURE<true>
{
	enum
	{
		value = 1
	};
};


template <int x>
struct static_assert_test
{
};


#if defined(__GNUC__) && (__GNUC__ == 3) && ((__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4))
#define STATIC_ASSERT(cond) \
	typedef char CXXABB_JOIN(static_assert_typedef_, __LINE__) \
        [CXXABB_STATIC_ASSERTION_FAILURE<(bool) (cond)>::value]
#else
#define STATIC_ASSERT(cond) \
	typedef static_assert_test<sizeof(CXXABB_STATIC_ASSERTION_FAILURE<(bool) (cond)>)> \
			CXXABB_JOIN(static_assert_typedef_, __LINE__)
#endif

#endif /* CXXABB_CORE_DEBUG_H_ */

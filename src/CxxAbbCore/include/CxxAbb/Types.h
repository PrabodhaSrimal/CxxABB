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
 * Types.h
 *
 * FileId      : $Id: Types.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 12, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Data type wrappers
 *
 */

#ifndef CXXABB_CORE_TYPES_H_
#define CXXABB_CORE_TYPES_H_

#include <cstring>
#include <string>
#include <climits>
#include <cstddef>
#include <sstream>

namespace CxxAbb
{

typedef unsigned char			UChar;
typedef unsigned short		UShort;
typedef unsigned long 		ULong;
typedef unsigned char			Byte;
using std::size_t;

#if defined(__GNUC__) || defined(__clang__)

	typedef signed char			Int8;
	typedef unsigned char			UInt8;
	typedef signed short			Int16;
	typedef unsigned short		UInt16;
	typedef signed int			Int32;
	typedef unsigned int			UInt32;
	typedef signed long			PtrT;
	typedef unsigned long			UPtrT;
	typedef std::ptrdiff_t 			PtrDiffT;

#if defined(__LP64__)
	#define CXXABB_PTR_IS_64_BIT 1
	#define CXXABB_LONG_IS_64_BIT 1
	typedef signed long        		Int64;
	typedef unsigned long      		UInt64;
#else
	typedef signed long long   		Int64;
	typedef unsigned long long 		UInt64;
#endif

	#define CXXABB_64_BIT_ENABLED	1

#elif defined(_MSC_VER)

	typedef signed char            Int8;
	typedef unsigned char          UInt8;
	typedef signed short           Int16;
	typedef unsigned short         UInt16;
	typedef signed int             Int32;
	typedef unsigned int           UInt32;
	typedef signed __int64         Int64;
	typedef unsigned __int64       UInt64;
	#if defined(_WIN64)
		#define CXXABB_PTR_IS_64_BIT 1
		typedef signed __int64     PtrT;
		typedef unsigned __int64   UPtrT;
	#else
		typedef signed long        PtrT;
		typedef unsigned long      UPtrT;
	#endif
	typedef std::ptrdiff_t			PtrDiffT;

	#define CXXABB_64_BIT_ENABLED	1

#endif

    template <typename T, typename U> struct AssertAreSameType;
    template <typename T> struct AssertAreSameType<T,T> {enum{value=1};};
    template <typename T, typename U> struct AssertAreNotSameType {enum{value=1}; };
    template <typename T> struct AssertAreNotSameType<T,T> {};

} /* namespace CxxAbb */

/** @brief Assert if given types are in same type hierarchy
 */
#define ASSERT_ARE_SAME_TYPE(type1, type2) \
        typedef char \
        	CXXABB_JOIN(SAME_TYPE, __LINE__)[::CxxAbb::AssertAreSameType<type1,type2>::value]

/** @brief Assert if given types are not in same type hierarchy
 */
#define ASSERT_ARE_NOT_SAME_TYPE(type1, type2) \
        typedef char \
        	CXXABB_JOIN(NOT_SAME_TYPE, __LINE__)[::CxxAbb::AssertAreNotSameType<type1,type2>::value]

/** @brief Assert the given type id POD (Plain Old Data) or has simple memory layout.
 *  If so the type can be use in memcpy, memmove, etc.
 */
#define ASSERT_POD(type)   \
    union  CXXABB_JOIN(IS_POD,__LINE__) { type TYPE_NOT_STANDARD_LAYOUT; };  \
    typedef char BOOST_JOIN(IS_POD2,__LINE__)[sizeof(BOOST_JOIN(IS_POD,__LINE__))];

#include <CxxAbb/NullType.h>

#endif /* CXXABB_CORE_TYPES_H_ */

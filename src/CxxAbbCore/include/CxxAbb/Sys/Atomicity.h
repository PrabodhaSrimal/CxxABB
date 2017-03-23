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
 * Atomicity.h
 *
 * FileId      : $Id: Atomicity.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : System
 * Comment     : Atomic operations
 *
 */

#ifndef CXXABB_CORE_ATOMICITY_H_
#define CXXABB_CORE_ATOMICITY_H_

#include <CxxAbb/Core.h>

namespace CxxAbb
{
namespace Sys
{

/// AtomicType defines platform specific int type, which natively support atomic operations

#ifdef CXXABB_DEF_ATOMICITY_GCC_X86

typedef int AtomicType;

#elif defined(CXXABB_DEF_ATOMICITY_GCC_X86_64)

typedef Int64 AtomicType;

#elif defined(CXXABB_DEF_ATOMICITY_WINDOWS)

typedef long AtomicType;

#elif defined(CXXABB_DEF_ATOMICITY_PTHREAD)

typedef int AtomicType;

#elif CXXABB_OS == CXXABB_OS_LINUX

#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1) || __GNUC__ > 4)

typedef int AtomicType;

#elif defined(__GNUC__)

#if (CXXABB_ARCH == CXXABB_ARCH_IA32)

typedef int AtomicType;

#elif (CXXABB_ARCH == CXXABB_ARCH_IA64) || (CXXABB_ARCH == CXXABB_ARCH_AMD64)

typedef Int64 AtomicType;

#endif

#endif

#elif CXXABB_OS == CXXABB_OS_WINDOWS

typedef long AtomicType;

#else

typedef int AtomicType;

#endif

/** @brief Atomic counter using atomic int type
 *
 *
 */
class CXXABB_API AtomicCounter
{
public:
	/// Counter with 0 value
	AtomicCounter();

	/// Counter with value
	explicit AtomicCounter(AtomicType _value);

	/// Counter with value
	AtomicCounter(unsigned int _value);


	/// Copy Constructor
	AtomicCounter(const AtomicCounter & _counter);

	/// Destructor
	~AtomicCounter();

	/// Cast to AtomicWord
	operator AtomicType() const;

	/// Get the value atomically
	AtomicType Value() const;

	/// Operators
	AtomicCounter& operator = (const AtomicCounter& _counter);
	AtomicCounter& operator = (AtomicType _atomictype);

	AtomicType operator ++ ();
	AtomicType operator ++ (Int32);
	AtomicType operator -- ();
	AtomicType operator -- (Int32);

	bool operator ! () const;

private:
	volatile AtomicType t_Counter;

};

} /* namespace Sys */
} /* namespace CxxAbb */


#endif /* CXXABB_CORE_ATOMICITY_H_ */

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
 * Atomicity.gcc.x86.cpp
 *
 * FileId      : $Id: Atomicity.gcc.x86.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Atomic Operations in GCC x86 - General with inline assembly
 *
 */

#include <CxxAbb/Sys/Atomicity.h>

namespace CxxAbb
{
namespace Sys
{

AtomicCounter::AtomicCounter()
		: t_Counter(0)
{
}

AtomicCounter::AtomicCounter(const AtomicCounter& _counter)
		: t_Counter(_counter.Value())
{
}

AtomicCounter::AtomicCounter(AtomicType _value)
		: t_Counter(_value)
{
}

AtomicCounter::AtomicCounter(unsigned int _value)
	: t_Counter(_value)
{
}

AtomicCounter::~AtomicCounter()
{
}

inline AtomicCounter::operator AtomicType() const
{
	asm volatile ( "lock; addl $0,0(%%esp)" : : : "memory" );
	return t_Counter;
}

inline AtomicType AtomicCounter::Value() const
{
	asm volatile ( "lock; addl $0,0(%%esp)" : : : "memory" );
	return t_Counter;
}

/// Operators
inline AtomicCounter& AtomicCounter::operator =(const AtomicCounter& _counter)
{
	volatile register Int32 tmp;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(tmp), "=m"(t_Counter)
			: "0"(_counter.Value()), "m"(t_Counter) );

	return *this;
}

inline AtomicCounter& AtomicCounter::operator =(AtomicType _atomictype)
{
	volatile register Int32 tmp;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(tmp), "=m"(t_Counter)
			: "0"(_atomictype), "m"(t_Counter) );

	return *this;
}

inline AtomicType AtomicCounter::operator ++()
{
	volatile register Int32 tmp;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(tmp), "=m"(t_Counter)
			: "0"(1), "m"(t_Counter) );

	return (tmp + 1);
}

inline AtomicType AtomicCounter::operator ++(Int32)
{
	volatile register Int32 ret;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(ret), "=m"(t_Counter)
			: "0"(1), "m"(t_Counter) );

	return ret;
}

inline AtomicType AtomicCounter::operator --()
{
	volatile register Int32 tmp;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(tmp), "=m"(t_Counter)
			: "0"(-1), "m"(t_Counter) );

	return (tmp - 1);
}

inline AtomicType AtomicCounter::operator --(Int32)
{
	volatile register Int32 tmp;

	asm volatile ( "lock; xaddl %0, %1"
			: "=r"(tmp), "=m"(t_Counter)
			: "0"(-1), "m"(t_Counter) );

	return tmp;
}

inline bool AtomicCounter::operator !() const
{
	return t_Counter == 0;
}

} /* namespace Sys */
} /* namespace CxxAbb */


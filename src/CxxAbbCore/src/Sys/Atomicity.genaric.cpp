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
 * Atomicity.genaric.cpp
 *
 * FileId      : $Id: Atomicity.genaric.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Atomic Operations for general architecture; using mutex
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
	return t_Counter;
}

inline AtomicCounter::operator int() const
{
	return t_Counter;
}

inline AtomicType AtomicCounter::Value() const
{
	return t_Counter;
}

/// Operators
inline AtomicCounter& AtomicCounter::operator =(const AtomicCounter& _counter)
{
	t_Counter = _counter.Value();
	return *this;
}

inline AtomicCounter& AtomicCounter::operator =(AtomicType _atomictype)
{
	t_Counter = _atomictype;
	return *this;
}

inline AtomicCounter& AtomicCounter::operator =(Int32 _intval)
{
	t_Counter = _intval;
	return *this;
}

inline AtomicType AtomicCounter::operator ++()
{
	t_Counter++;
	return *this;
}

inline AtomicType AtomicCounter::operator ++(Int32)
{
	++t_Counter;
	return *this;
}

inline AtomicType AtomicCounter::operator --()
{
	t_Counter--;
	return *this;
}

inline AtomicType AtomicCounter::operator --(Int32)
{
	--t_Counter;
	return *this;
}

inline bool AtomicCounter::operator !() const
{
	return t_Counter == 0;
}

} /* namespace Sys */
} /* namespace CxxAbb */



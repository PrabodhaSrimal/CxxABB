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
 * Atomicity.win.cpp
 *
 * FileId      : $Id: Atomicity.win.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Atomic Operations in Windows
 *
 */

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>

#include <CxxAbb/Atomicity.h>

namespace CxxAbb
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
#if (_MSC_VER >= 1400)
	MemoryBarrier();
#endif
	return t_Counter;
}

inline AtomicType AtomicCounter::Value() const
{
#if (_MSC_VER >= 1400)
	MemoryBarrier();
#endif
	return t_Counter;
}

/// Operators
inline AtomicCounter& AtomicCounter::operator =(const AtomicCounter& _counter)
{
	InterlockedExchange(&t_Counter, _counter.Value());
	return *this;
}

inline AtomicCounter& AtomicCounter::operator =(AtomicType _atomictype)
{
	InterlockedExchange(&t_Counter, _atomictype);
	return *this;
}

inline AtomicType AtomicCounter::operator ++()
{
	return InterlockedIncrement(&t_Counter);
}

inline AtomicType AtomicCounter::operator ++(Int32)
{
	AtomicType result = InterlockedIncrement(&t_Counter);
	return --result;
}

inline AtomicType AtomicCounter::operator --()
{
	return InterlockedDecrement(&t_Counter);
}

inline AtomicType AtomicCounter::operator --(Int32)
{
	AtomicType result = InterlockedDecrement(&t_Counter);
	return ++result;
}

inline bool AtomicCounter::operator !() const
{
	return t_Counter == 0;
}

} /* namespace CxxAbb */


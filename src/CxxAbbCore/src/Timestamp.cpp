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
 * Timestamp.cpp
 *
 * FileId      : $Id: Timestamp.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 17, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Timestamp representation
 *
 */

#include <CxxAbb/Timestamp.h>

namespace CxxAbb
{

Timestamp::Timestamp()
{
	Now();
}

Timestamp::Timestamp(TimeVal _timeval)
	: t_Value(_timeval)
{}

Timestamp::Timestamp(const Timestamp& _other)
{
	t_Value = _other.t_Value;
}

Timestamp::~Timestamp()
{
}

std::time_t Timestamp::Epoch() const
{
	return (t_Value/MuSecPerSecond);
}

Timestamp::UtcTimeVal Timestamp::Utc() const
{
	return ((t_Value * 10) + (TimeDiff(0x01b21dd2) << 32) + 0x13814000);
}

Timestamp::TimeVal Timestamp::EpochMicroseconds() const
{
	return t_Value;
}

Timestamp::TimeDiff Timestamp::Elapsed() const
{
	return Timestamp() - *this;
}

bool Timestamp::IsElapsed(TimeDiff _interval) const
{
	TimeDiff tDiff = Timestamp() - *this;
	return (tDiff >= _interval);
}

Timestamp Timestamp::FromEpoch(std::time_t _epoch)
{
	return Timestamp(TimeVal(_epoch) * MuSecPerSecond);
}


Timestamp Timestamp::FromUtc(UtcTimeVal _utc)
{
	_utc -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	_utc /= 10;
	return Timestamp(_utc);
}

} /* namespace CxxAbb */

#if (CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_UNIX) || (CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_BSD)

#include "posix/Timestamp.cpp"

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_WINDOWS

#include "win32/Timestamp.cpp"

#endif

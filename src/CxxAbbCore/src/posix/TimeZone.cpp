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
 * TimeZone.cpp
 *
 * FileId      : $Id: TimeZone.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 25, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : TimeZone check functionality - POSIX implementation
 *
 */

#include <CxxAbb/TimeZone.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Sys/Mutex.h>
#include <CxxAbb/Sys/ScopedLock.h>
#include <ctime>
#include <sys/time.h>

namespace CxxAbb
{

static CxxAbb::Sys::Mutex mtx_EnvTZSet; // global lock for TZ env var
static std::string s_DefaultTzName; // default local TZ value

class UnixTZ
{
public:
	UnixTZ()
	{
		const char * z = getenv("TZ");
		if(z != NULL)
		{
			s_DefaultTzName = std::string(z);
		}

		tzset();
	}

	int Offset()
	{
#if CXXABB_OS == CXXABB_OS_CYGWIN
		return (-_timezone);
#else
		return (-timezone);
#endif
	}

	const char * Name(bool _bDst)
	{
		return tzname[(_bDst ? 1 : 0)];
	}

	bool Dst(std::time_t _t)
	{
		std::time_t now = _t;
		struct std::tm t;
		localtime_r(&now, &t);
		return t.tm_isdst == 1 ? true : false;
	}
};


static UnixTZ Sm_UnizTZ;

int TimeZone::UtcOffset()
{
	return Sm_UnizTZ.Offset();
}

int TimeZone::DstCorrection()
{
	return Sm_UnizTZ.Dst(time(NULL)) ? 3600 : 0;
}

bool TimeZone::IsDst(const CxxAbb::Timestamp& _timestamp)
{
	return Sm_UnizTZ.Dst(_timestamp.Epoch());
}

std::string TimeZone::TimeZoneName()
{
	return std::string(Sm_UnizTZ.Name(Sm_UnizTZ.Dst(std::time(NULL))));
}

std::string TimeZone::TimeZoneNameStandard()
{
	return std::string(Sm_UnizTZ.Name(0));
}

std::string TimeZone::TimeZoneNameDst()
{
	return std::string(Sm_UnizTZ.Name(1));
}


TimeZoneInfo::TimeZoneInfo(const char * _zZoneName)
	: s_Zone(_zZoneName)
{
}

TimeZoneInfo::TimeZoneInfo(const TimeZoneInfo & _rhs)
{
	s_Zone = _rhs.s_Zone;
}

int TimeZoneInfo::GetTimeZoneDiff(time_t _epoch)
{
	struct tm lcl;

	{
		CxxAbb::Sys::ScopedLock<CxxAbb::Sys::Mutex> l(mtx_EnvTZSet);
		SetTz();

		localtime_r(&_epoch, &lcl);

		ResetTz();
	}
	return lcl.tm_gmtoff;
}

bool TimeZoneInfo::IsDST(time_t _epoch)
{
	struct tm lcl;

	{
		CxxAbb::Sys::ScopedLock<CxxAbb::Sys::Mutex> l(mtx_EnvTZSet);
		SetTz();

		localtime_r(&_epoch, &lcl);

		ResetTz();
	}
	return lcl.tm_isdst;
}

void TimeZoneInfo::ResetTz()
{
	if(!s_DefaultTzName.empty())
	{
		setenv("TZ", s_DefaultTzName.c_str(), 1);
	}
	else
	{
		unsetenv("TZ");
	}
	tzset();
}

void TimeZoneInfo::SetTz()
{
	if(!s_Zone.empty())
	{
		setenv("TZ", s_Zone.c_str(), 1);
		tzset();
	}
}

}  /* namespace CxxAbb */


 

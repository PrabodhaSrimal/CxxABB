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
 * LocalDateTime.cpp
 *
 * FileId      : $Id: LocalDateTime.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 1, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Timezone aware datetime representation
 *
 */

#include <CxxAbb/LocalDateTime.h>
#include <CxxAbb/Debug.h>
#include <cmath>

namespace CxxAbb
{

LocalDateTime::LocalDateTime(const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	Timestamp mNow;
	t_UtcTime = mNow.Utc();
	// current t_UtcTime is in TimeZone, adjust it to UTC
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
	Update();
}

LocalDateTime::LocalDateTime(DateTime & _dt, const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	t_UtcTime = _dt.UtcTime();
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
	Update();
}

LocalDateTime::LocalDateTime(CxxAbb::Timestamp _timestamp,
	const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone),
	  t_UtcTime(_timestamp.Utc())
{
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

LocalDateTime::LocalDateTime(std::time_t _tEpochSecs, const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	Timestamp ts(_tEpochSecs * MSecsPerSecond);
	t_UtcTime = ts.Utc();
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

LocalDateTime::LocalDateTime(double _dEpochMsecs, const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	Timestamp ts(static_cast<Timestamp::TimeVal>(_dEpochMsecs * MuSecsPerMSecond));
	t_UtcTime = ts.Utc();
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

LocalDateTime::LocalDateTime(short _iYear, short _iMonth, short _iDay, short _iHour,
	short _iMinute, short _iSecond, short _iMSecond, short _iMuSecond,
	const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	t_UtcTime = Julian2Utc(Gregorian2Julian(_iYear, _iMonth, _iDay))
		+ 10 * (_iHour * MuSecsPerHour + _iMinute * MuSecsPerMinute + _iSecond * MuSecsPerSecond
				+ _iMSecond * MuSecsPerMSecond + _iMuSecond);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	i_Year = _iYear;
	i_Month = _iMonth;
	i_Day = _iDay;
	i_Hour = _iHour;
	i_Minute = _iMinute;
	i_Second = _iSecond;
	i_MSecond = _iMSecond;
	i_MuSecond = _iMuSecond;

	ASSERT(_iYear >= 0 && _iYear <= 9999);
	ASSERT(_iMonth >= 1 && _iMonth <= MONTH_COUNT);
	ASSERT(_iDay >= 1 && _iDay <= DaysOfMonth(_iYear, _iMonth));
	ASSERT(_iHour >= 0 && (unsigned)_iHour <= MaxHours);
	ASSERT(_iMinute >= 0 && (unsigned)_iMinute <= MaxMinutes);
	ASSERT(_iSecond >= 0 && (unsigned)_iSecond <= MaxSeconds);
	ASSERT(_iMSecond >= 0 && (unsigned)_iMSecond <= MaxMSecs);
	ASSERT(_iMuSecond >= 0 && (unsigned)_iMuSecond <= MaxMuSecs);
}

LocalDateTime::LocalDateTime(Timestamp::UtcTimeVal _utcTime, Timestamp::TimeDiff _diff,
	const char * _zZone /*= LOCAL_TIMEZONE*/)
	: m_ZoneInfo(_zZone)
{
	t_UtcTime = _utcTime + _diff*10;
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

LocalDateTime::LocalDateTime(const LocalDateTime & _rhs)
{
	m_ZoneInfo = _rhs.m_ZoneInfo;
	i_Tzd = _rhs.i_Tzd;
	t_UtcTime = _rhs.t_UtcTime;
	i_Year = _rhs.i_Year;
	i_Month = _rhs.i_Month;
	i_Day = _rhs.i_Day;
	i_Hour = _rhs.i_Hour;
	i_Minute = _rhs.i_Minute;
	i_Second = _rhs.i_Second;
	i_MSecond = _rhs.i_MSecond;
	i_MuSecond = _rhs.i_MuSecond;
}

LocalDateTime::~LocalDateTime()
{
}

std::string LocalDateTime::WeekDayName() const
{
	static const char * dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
		"Friday", "Saturday"};

	return std::string(dayNames[DayOfWeek()]);
}

std::string LocalDateTime::WeekDayShortName() const
{
	static const char * dayShortNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	return std::string(dayShortNames[DayOfWeek()]);
}

std::string LocalDateTime::MonthName() const
{
	static const char * monthNames[] = {"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

	return std::string(monthNames[i_Month - 1]);
}

std::string LocalDateTime::MonthShortName() const
{
	static const char * monthShortNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec"};

	return std::string(monthShortNames[i_Month - 1]);
}

short LocalDateTime::Week(DaysOfWeek _eFirstDayOfWeek) const
{
	/// find the first firstDayOfWeek of current year
	int baseDay = 1;
	while (LocalDateTime(i_Year, 1, baseDay).DayOfWeek() != _eFirstDayOfWeek)
		++baseDay;

	int doy  = DayOfYear();
	// check if we are at week 0 or 1
	int offs = baseDay <= 4 ? 0 : 1;
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay)/7 + 1 + offs;
}

short LocalDateTime::DayOfYear() const
{
	int doy = 0;
	for (int month = 1; month < i_Month; ++month)
		doy += DaysOfMonth(i_Year, month);
	doy += i_Day;
	return doy;
}

LocalDateTime::DaysOfWeek LocalDateTime::DayOfWeek() const
{
	return DaysOfWeek(int((std::floor(JulianDayNumber() + 1.5))) % 7);
}

bool LocalDateTime::IsLeapYear(int _year)
{
	return ((_year % 4) == 0) && ((_year % 100) != 0 || (_year % 400) == 0);
}

int LocalDateTime::DaysOfMonth(int _year, int _month)
{
	ASSERT(_month >= 1 && _month <= 12);

	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (_month == 2 && IsLeapYear(_year))
		return 29;
	else
		return daysOfMonthTable[_month];
}

bool LocalDateTime::IsValid(short _iYear, short _iMonth, short _iDay, short _iHour,
	short _iMinute, short _iSecond, short _iMSecond, short _iMuSecond)
{
	return
		(_iYear >= 0 && _iYear <= 9999) &&
		(_iMonth >= 1 && _iMonth <= MONTH_COUNT) &&
		(_iDay >= 1 && _iDay <= DaysOfMonth(_iYear, _iMonth)) &&
		(_iHour >= 0 && (unsigned)_iHour <= MaxHours) &&
		(_iMinute >= 0 && (unsigned)_iMinute <= MaxMinutes) &&
		(_iSecond >= 0 && (unsigned)_iSecond <= MaxSeconds) &&
		(_iMSecond >= 0 && (unsigned)_iMSecond <= MaxMSecs) &&
		(_iMuSecond >= 0 && (unsigned)_iMuSecond <= MaxMuSecs);
}

LocalDateTime::JulianDay LocalDateTime::Utc2Julian(CxxAbb::Timestamp::UtcTimeVal _utc)
{
	JulianDay dUtcDays = _utc/UtcUnitsPerDay; // JDs since gregorian calendar reform
	return (dUtcDays + JulianDayOfGregReform);
}

LocalDateTime::JulianDay LocalDateTime::Gregorian2Julian(short _iYear, short _iMonth,
	short _iDay, short _iHour, short _iMinute, short _iSecond, short _iMSecond,
	short _iMuSecond)
{
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	// days in previous months
	static int lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};

	// day as a fraction
	double dday = double(_iDay) + 	((double((_iHour*MinutesPerHour + _iMinute)*SecondsPerMinute
		+ _iSecond)*MSecsPerSecond + _iMSecond)*MuSecsPerMSecond + _iMuSecond)/MuSecsPerDay;

	// Re-calculate month number to easy the leap day calculation.
	// so the first month of the year is March. Making Feb as last month.
	if (_iMonth < March)
	{
		_iMonth += MONTH_COUNT;
		--_iYear;
	}
	double dyear = double(_iYear);

	// there are 365 days in a year plus an extra day for leap years, where leap years occur every
	// 4 years except for century years unless they are also divisible by 400
	return dday + lookup[_iMonth] + 365*_iYear + std::floor(dyear/4) - std::floor(dyear/100)
	+ std::floor(dyear/400) + JulianDayOfGregDay0;
}

LocalDateTime::JulianDay LocalDateTime::Gregorian2Julian2(short _iYear, short _iMonth,
	short _iDay, short /*_iHour*/, short /*_iMinute*/, short /*_iSecond*/, short /*_iMSecond*/,
	short /*_iMuSecond*/)
{
	return (1461 * (_iYear + 4800 + (_iMonth - 14) / 12)) / 4
		+ (367 * (_iMonth - 2 - 12 * ((_iMonth - 14) / 12))) / 12
		- (3 * ((_iYear + 4900 + (_iMonth - 14) / 12) / 100)) / 4 + _iDay - 32075;
}

CxxAbb::Timestamp::UtcTimeVal LocalDateTime::Julian2Utc(LocalDateTime::JulianDay _jd)
{
	return Timestamp::UtcTimeVal((_jd - JulianDayOfGregReform) * UtcUnitsPerDay);
}

void LocalDateTime::Julian2Gregorian(LocalDateTime::JulianDay _jd)
{
	double z    = std::floor(_jd - JulianDayOfGregDay0); // get base JD (base is March 1 Year 0)
	double r    = _jd - JulianDayOfGregDay0 - z;
	double g    = z - 0.25;
	double a    = std::floor(g / 36524.25);
	double b    = a - std::floor(a/4);
	i_Year       = short(std::floor((b + g)/365.25));
	double c    = b + z - std::floor(365.25*i_Year);
	i_Month      = short(std::floor((5*c + 456)/153));
	double dday = c - std::floor((153.0*i_Month - 457)/5) + r;
	i_Day        = short(dday);
	if (i_Month > MONTH_COUNT)
	{
		++i_Year;
		i_Month -= MONTH_COUNT;
	}
	r      *= 24;
	i_Hour   = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	i_Minute = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	i_Second = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	i_MSecond = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	i_MuSecond = short(r + 0.5);

	Normalize();

	ASSERT(IsValid(i_Year,i_Month,i_Day,i_Hour,i_Minute,i_Second,i_MSecond,i_MuSecond));
}

void LocalDateTime::Julian2Gregorian2(LocalDateTime::JulianDay _jd)
{
	register int l, n, i, j;
	l = static_cast<int>(_jd + 68569);
	n = (4 * l) / 146097;
	l = l - (146097 * n + 3) / 4;
	i = (4000 * (l + 1)) / 1461001;
	l = l - (1461 * i) / 4 + 31;
	j = (80 * l) / 2447;
	i_Day = l - (2447 * j) / 80;
	l = j / 11;
	i_Month = j + 2 - (12 * l);
	i_Year = 100 * (n - 49) + i + l;
}

void LocalDateTime::Utc2Time(CxxAbb::Timestamp::UtcTimeVal _utc)
{
	Timespan span(_utc/10);
	i_Hour = span.Hours();
	i_Minute = span.Minutes();
	i_Second = span.Seconds();
	i_MSecond = span.Milliseconds();
	i_MuSecond = span.Microseconds();
}

void LocalDateTime::CorrectBounds(short& _lower, short& _upper, short _range)
{
	if (_lower >= _range)
	{
		_upper += short(_lower / _range);
		_lower   = short(_lower % _range);
	}
}

void LocalDateTime::Normalize()
{
	CorrectBounds(i_MuSecond, i_MSecond, MuSecsPerMSecond);
	CorrectBounds(i_MSecond, i_Second, MSecsPerSecond);
	CorrectBounds(i_Second, i_Minute, SecondsPerMinute);
	CorrectBounds(i_Minute, i_Hour, MinutesPerHour);
	CorrectBounds(i_Hour, i_Day, HoursPerDay);

	if (i_Day > DaysOfMonth(i_Year, i_Month))
	{
		i_Day -= DaysOfMonth(i_Year, i_Month);
		if (++i_Month > MONTH_COUNT)
		{
			++i_Year;
			i_Month -= MONTH_COUNT;
		}
	}
}

void LocalDateTime::Update()
{
	Julian2Gregorian(Utc2Julian(t_UtcTime));
	Utc2Time(t_UtcTime);
}

void LocalDateTime::Swap(LocalDateTime& _rhs)
{
	std::swap(t_UtcTime, _rhs.t_UtcTime);
	std::swap(i_Year, _rhs.i_Year);
	std::swap(i_Month, _rhs.i_Month);
	std::swap(i_Day, _rhs.i_Day);
	std::swap(i_Hour, _rhs.i_Hour);
	std::swap(i_Minute, _rhs.i_Minute);
	std::swap(i_Second, _rhs.i_Second);
	std::swap(i_MSecond, _rhs.i_MSecond);
	std::swap(i_MuSecond, _rhs.i_MuSecond);
	std::swap(i_Tzd, _rhs.i_Tzd);
	m_ZoneInfo.Swap(_rhs.m_ZoneInfo);
}

LocalDateTime & LocalDateTime::Set(short _iYear, short _iMonth, short _iDay, short _iHour,
		short _iMinute, short _iSecond, short _iMSecond, short _iMuSecond, const char * _zZone)
{
	ASSERT(_iYear >= 0 && _iYear <= 9999);
	ASSERT(_iMonth >= 1 && _iMonth <= MONTH_COUNT);
	ASSERT(_iDay >= 1 && _iDay <= DaysOfMonth(_iYear, _iMonth));
	ASSERT(_iHour >= 0 && (unsigned)_iHour <= MaxHours);
	ASSERT(_iMinute >= 0 && (unsigned)_iMinute <= MaxMinutes);
	ASSERT(_iSecond >= 0 && (unsigned)_iSecond <= MaxSeconds);
	ASSERT(_iMSecond >= 0 && (unsigned)_iMSecond <= MaxMSecs);
	ASSERT(_iMuSecond >= 0 && (unsigned)_iMuSecond <= MaxMuSecs);

	t_UtcTime = Julian2Utc(Gregorian2Julian(_iYear, _iMonth, _iDay))
			+ 10 * (_iHour * MuSecsPerHour + _iMinute * MuSecsPerMinute + _iSecond * MuSecsPerSecond
					+ _iMSecond * MuSecsPerMSecond + _iMuSecond);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	i_Year = _iYear;
	i_Month = _iMonth;
	i_Day = _iDay;
	i_Hour = _iHour;
	i_Minute = _iMinute;
	i_Second = _iSecond;
	i_MSecond = _iMSecond;
	i_MuSecond = _iMuSecond;

	m_ZoneInfo = TimeZoneInfo(_zZone);

	return *this;
}

LocalDateTime::JulianDay LocalDateTime::JulianDayNumber() const
{
	return Utc2Julian(t_UtcTime);
}

Timestamp::UtcTimeVal LocalDateTime::UtcTime() const
{
	return t_UtcTime - ((Timestamp::TimeDiff)i_Tzd)*UtcUnitsPerSecond;
}

Timestamp LocalDateTime::TimestampValue() const
{
	return Timestamp::FromUtc(t_UtcTime);
}

CxxAbb::DateTime LocalDateTime::UtcDateTime() const
{
	return DateTime(t_UtcTime, -((Timestamp::TimeDiff)i_Tzd) * UtcUnitsPerSecond);
}

LocalDateTime& LocalDateTime::operator = (const LocalDateTime& _rhs)
{
	if(&_rhs != this)
	{
		t_UtcTime = _rhs.t_UtcTime;
		i_Year = _rhs.i_Year;
		i_Month = _rhs.i_Month;
		i_Day = _rhs.i_Day;
		i_Hour = _rhs.i_Hour;
		i_Minute = _rhs.i_Minute;
		i_Second = _rhs.i_Second;
		i_MSecond = _rhs.i_MSecond;
		i_MuSecond = _rhs.i_MuSecond;

		i_Tzd = _rhs.i_Tzd;
		m_ZoneInfo = _rhs.m_ZoneInfo;
	}
	return *this;
}

LocalDateTime& LocalDateTime::operator = (const Timestamp& _rhs)
{
	if(this->TimestampValue() != _rhs)
	{
		t_UtcTime = _rhs.Utc();
		i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
		t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
		Update();
	}
	return *this;
}

LocalDateTime& LocalDateTime::operator = (LocalDateTime::JulianDay _jd)
{
	t_UtcTime = Julian2Utc(_jd);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
	Update();
	return *this;
}

bool LocalDateTime::operator == (const LocalDateTime& _dateTime) const
{
	return (UtcTime() == _dateTime.UtcTime());
}

bool LocalDateTime::operator != (const LocalDateTime& _dateTime) const
{
	return (UtcTime() != _dateTime.UtcTime());
}

bool LocalDateTime::operator <  (const LocalDateTime& _dateTime) const
{
	return (UtcTime() < _dateTime.UtcTime());
}

bool LocalDateTime::operator <= (const LocalDateTime& _dateTime) const
{
	return (UtcTime() <= _dateTime.UtcTime());
}

bool LocalDateTime::operator >  (const LocalDateTime& _dateTime) const
{
	return (UtcTime() > _dateTime.UtcTime());
}

bool LocalDateTime::operator >= (const LocalDateTime& _dateTime) const
{
	return (UtcTime() >= _dateTime.UtcTime());
}

LocalDateTime  LocalDateTime::operator +  (const Timespan& _span) const
{
	DateTime dt(UtcTime(), _span.TotalMicroseconds());

	return LocalDateTime(dt, m_ZoneInfo.GetZoneName());
}

LocalDateTime  LocalDateTime::operator -  (const Timespan& _span) const
{
	DateTime dt(UtcTime(), -_span.TotalMicroseconds());

	return LocalDateTime(dt, m_ZoneInfo.GetZoneName());
}

Timespan  LocalDateTime::operator -  (const LocalDateTime& _dateTime) const
{
	return Timespan((UtcTime() - _dateTime.UtcTime())/10);
}

LocalDateTime& LocalDateTime::operator += (const Timespan& _span)
{
	t_UtcTime = UtcTime() + (_span.TotalMicroseconds() * 10);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
	Update();
	return *this;
}

LocalDateTime& LocalDateTime::operator -= (const Timespan& _span)
{
	t_UtcTime = UtcTime() - (_span.TotalMicroseconds() * 10);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	t_UtcTime += (i_Tzd * UtcUnitsPerSecond);
	Update();
	return *this;
}

void LocalDateTime::AddMicroSeconds(CxxAbb::Int64 _iMuSecs)
{
	t_UtcTime += (_iMuSecs * 10);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddMilliSeonds(CxxAbb::Int64 _iMSecs)
{
	t_UtcTime += (_iMSecs * UtcUnitsPerMSec);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddSeconds(int _iSecs)
{
	t_UtcTime += (_iSecs * UtcUnitsPerSecond);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddMinutes(int _iMins)
{
	t_UtcTime += (_iMins * UtcUnitsPerMinute);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddHours(int _iHours)
{
	t_UtcTime += (_iHours * UtcUnitsPerHour);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddDays(int _iDays)
{
	t_UtcTime += (_iDays * UtcUnitsPerDay);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

void LocalDateTime::AddWeeks(int _iWeeks)
{
	t_UtcTime += (_iWeeks * 7 * UtcUnitsPerDay);
	i_Tzd = m_ZoneInfo.GetTimeZoneDiff(Timestamp::FromUtc(t_UtcTime).Epoch());
	Update();
}

//void LocalDateTime::AddMonths(int _iMonths)
//{
//	t_UtcTime += (_iMonths * UtcUnitsPerDay * 30); //FIXME: logic ???
//	Update();
//}

}  /* namespace CxxAbb */





 

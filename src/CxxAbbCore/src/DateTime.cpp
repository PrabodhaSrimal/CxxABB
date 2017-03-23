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
 * DateTime.cpp
 *
 * FileId      : $Id: DateTime.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 26, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Date and Time representation
 *
 */

#include <CxxAbb/DateTime.h>
#include <CxxAbb/Debug.h>
#include <cmath>

namespace CxxAbb
{

DateTime::DateTime()
{
	Timestamp mNow;
	t_UtcTime = mNow.Utc();
	Update();
}

DateTime::DateTime(CxxAbb::Timestamp _timestamp)
	: t_UtcTime(_timestamp.Utc())
{
	Update();
}

DateTime::DateTime(std::time_t _tEpochSecs)
{
	Timestamp ts(_tEpochSecs * MSecsPerSecond);
	t_UtcTime = ts.Utc();
	Update();
}

DateTime::DateTime(double _dEpochMsecs)
{
	Timestamp ts(static_cast<Timestamp::TimeVal>(_dEpochMsecs * MuSecsPerMSecond));
	t_UtcTime = ts.Utc();
	Update();
}

DateTime::DateTime(short _iYear, short _iMonth, short _iDay, short _iHour, short _iMinute,
		short _iSecond, short _iMSecond, short _iMuSecond)
{
	t_UtcTime = Julian2Utc(Gregorian2Julian(_iYear, _iMonth, _iDay))
		+ 10 * (_iHour * MuSecsPerHour + _iMinute * MuSecsPerMinute + _iSecond * MuSecsPerSecond
				+ _iMSecond * MuSecsPerMSecond + _iMuSecond);
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

DateTime::DateTime(Timestamp::UtcTimeVal _utcTime, Timestamp::TimeDiff _diff)
{
	t_UtcTime = _utcTime + _diff*10;
	Update();
}

DateTime::DateTime(const DateTime & _rhs)
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
}

DateTime::~DateTime()
{
}

std::string DateTime::WeekDayName() const
{
	static const char * dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
		"Friday", "Saturday"};

	return std::string(dayNames[DayOfWeek()]);
}

std::string DateTime::WeekDayShortName() const
{
	static const char * dayShortNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	return std::string(dayShortNames[DayOfWeek()]);
}

std::string DateTime::MonthName() const
{
	static const char * monthNames[] = {"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

	return std::string(monthNames[i_Month - 1]);
}

std::string DateTime::MonthShortName() const
{
	static const char * monthShortNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec"};

	return std::string(monthShortNames[i_Month - 1]);
}

short DateTime::Week(DaysOfWeek _eFirstDayOfWeek) const
{
	/// find the first firstDayOfWeek of current year
	int baseDay = 1;
	while (DateTime(i_Year, 1, baseDay).DayOfWeek() != _eFirstDayOfWeek) ++baseDay;

	int doy  = DayOfYear();
	// check if we are at week 0 or 1
	int offs = baseDay <= 4 ? 0 : 1;
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay)/7 + 1 + offs;
}

short DateTime::DayOfYear() const
{
	int doy = 0;
	for (int month = 1; month < i_Month; ++month)
		doy += DaysOfMonth(i_Year, month);
	doy += i_Day;
	return doy;
}

DateTime::DaysOfWeek DateTime::DayOfWeek() const
{
	return DaysOfWeek(int((std::floor(JulianDayNumber() + 1.5))) % 7);
}

bool DateTime::IsLeapYear(int _year)
{
	return ((_year % 4) == 0) && ((_year % 100) != 0 || (_year % 400) == 0);
}

int DateTime::DaysOfMonth(int _year, int _month)
{
	ASSERT(_month >= 1 && _month <= 12);

	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (_month == 2 && IsLeapYear(_year))
		return 29;
	else
		return daysOfMonthTable[_month];
}

bool DateTime::IsValid(short _iYear, short _iMonth, short _iDay, short _iHour, short _iMinute,
	short _iSecond, short _iMSecond, short _iMuSecond)
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

DateTime::JulianDay DateTime::Utc2Julian(CxxAbb::Timestamp::UtcTimeVal _utc)
{
	JulianDay dUtcDays = JulianDay(_utc)/double(UtcUnitsPerDay); // JDs since gregorian calendar reform
	return (dUtcDays + JulianDayOfGregReform);
}

DateTime::JulianDay DateTime::Gregorian2Julian(short _iYear, short _iMonth, short _iDay,
	short _iHour, short _iMinute, short _iSecond, short _iMSecond, short _iMuSecond)
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

DateTime::JulianDay DateTime::Gregorian2Julian2(short _iYear, short _iMonth, short _iDay,
	short /*_iHour*/, short /*_iMinute*/, short /*_iSecond*/, short /*_iMSecond*/,
	short /*_iMuSecond*/)
{
	return (1461 * (_iYear + 4800 + (_iMonth - 14) / 12)) / 4
		+ (367 * (_iMonth - 2 - 12 * ((_iMonth - 14) / 12))) / 12
		- (3 * ((_iYear + 4900 + (_iMonth - 14) / 12) / 100)) / 4 + _iDay - 32075;
}

CxxAbb::Timestamp::UtcTimeVal DateTime::Julian2Utc(DateTime::JulianDay _jd)
{
	return Timestamp::UtcTimeVal((_jd - JulianDayOfGregReform) * UtcUnitsPerDay);
}

void DateTime::Julian2Gregorian(DateTime::JulianDay _jd)
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

void DateTime::Julian2Gregorian2(DateTime::JulianDay _jd)
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

void DateTime::Utc2Time(CxxAbb::Timestamp::UtcTimeVal _utc)
{
	Timespan span(_utc/10);
	i_Hour = span.Hours();
	i_Minute = span.Minutes();
	i_Second = span.Seconds();
	i_MSecond = span.Milliseconds();
	i_MuSecond = span.Microseconds();
}

void DateTime::CorrectBounds(short& _lower, short& _upper, short _range)
{
	if (_lower >= _range)
	{
		_upper += short(_lower / _range);
		_lower   = short(_lower % _range);
	}
}

void DateTime::Normalize()
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

void DateTime::Update()
{
	Julian2Gregorian(Utc2Julian(t_UtcTime));
	Utc2Time(t_UtcTime);
}

void DateTime::Swap(DateTime& _rhs)
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
}

DateTime & DateTime::Set(short _iYear, short _iMonth, short _iDay, short _iHour,
		short _iMinute, short _iSecond, short _iMSecond, short _iMuSecond)
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
	i_Year = _iYear;
	i_Month = _iMonth;
	i_Day = _iDay;
	i_Hour = _iHour;
	i_Minute = _iMinute;
	i_Second = _iSecond;
	i_MSecond = _iMSecond;
	i_MuSecond = _iMuSecond;

	return *this;
}

DateTime::JulianDay DateTime::JulianDayNumber() const
{
	return Utc2Julian(t_UtcTime);
}

Timestamp::UtcTimeVal DateTime::UtcTime() const
{
	return t_UtcTime;
}

Timestamp DateTime::TimestampValue() const
{
	return Timestamp::FromUtc(t_UtcTime);
}

DateTime& DateTime::operator = (const DateTime& _rhs)
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
	}
	return *this;
}

DateTime& DateTime::operator = (const Timestamp& _rhs)
{
	t_UtcTime = _rhs.Utc();
	Update();
	return *this;
}

DateTime& DateTime::operator = (DateTime::JulianDay _jd)
{
	t_UtcTime = Julian2Utc(_jd);
	Update();
	return *this;
}

bool DateTime::operator == (const DateTime& _dateTime) const
{
	return (t_UtcTime == _dateTime.t_UtcTime);
}

bool DateTime::operator != (const DateTime& _dateTime) const
{
	return (t_UtcTime != _dateTime.t_UtcTime);
}

bool DateTime::operator <  (const DateTime& _dateTime) const
{
	return (t_UtcTime < _dateTime.t_UtcTime);
}

bool DateTime::operator <= (const DateTime& _dateTime) const
{
	return (t_UtcTime <= _dateTime.t_UtcTime);
}

bool DateTime::operator >  (const DateTime& _dateTime) const
{
	return (t_UtcTime > _dateTime.t_UtcTime);
}

bool DateTime::operator >= (const DateTime& _dateTime) const
{
	return (t_UtcTime >= _dateTime.t_UtcTime);
}

DateTime  DateTime::operator +  (const Timespan& _span) const
{
	return DateTime(t_UtcTime, _span.TotalMicroseconds());
}

DateTime  DateTime::operator -  (const Timespan& _span) const
{
	return DateTime(t_UtcTime, -_span.TotalMicroseconds());
}

Timespan  DateTime::operator -  (const DateTime& _dateTime) const
{
	return Timespan((t_UtcTime - _dateTime.t_UtcTime)/10);
}

DateTime& DateTime::operator += (const Timespan& _span)
{
	t_UtcTime += (_span.TotalMicroseconds() * 10);
	Update();
	return *this;
}

DateTime& DateTime::operator -= (const Timespan& _span)
{
	t_UtcTime -= (_span.TotalMicroseconds() * 10);
	Update();
	return *this;
}

void DateTime::AddMicroSeconds(CxxAbb::Int64 _iMuSecs)
{
	t_UtcTime += (_iMuSecs * 10);
	Update();
}

void DateTime::AddMilliSeonds(CxxAbb::Int64 _iMSecs)
{
	t_UtcTime += (_iMSecs * UtcUnitsPerMSec);
	Update();
}

void DateTime::AddSeconds(int _iSecs)
{
	t_UtcTime += (_iSecs * UtcUnitsPerSecond);
	Update();
}

void DateTime::AddMinutes(int _iMins)
{
	t_UtcTime += (_iMins * UtcUnitsPerMinute);
	Update();
}

void DateTime::AddHours(int _iHours)
{
	t_UtcTime += (_iHours * UtcUnitsPerHour);
	Update();
}

void DateTime::AddDays(int _iDays)
{
	t_UtcTime += (_iDays * UtcUnitsPerDay);
	Update();
}

void DateTime::AddWeeks(int _iWeeks)
{
	t_UtcTime += (_iWeeks * 7 * UtcUnitsPerDay);
	Update();
}

//void DateTime::AddMonths(int _iMonths)
//{
//	t_UtcTime += (_iMonths * UtcUnitsPerDay * 30); //FIXME: logic ???
//	Update();
//}

}  /* namespace CxxAbb */



 

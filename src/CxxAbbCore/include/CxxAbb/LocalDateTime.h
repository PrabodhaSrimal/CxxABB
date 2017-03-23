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
 * LocalDateTime.h
 *
 * FileId      : $Id: LocalDateTime.h 20 2012-11-22 07:46:58Z prabodar $
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

#ifndef CXXABB_CORE_LOCALDATETIME_H_
#define CXXABB_CORE_LOCALDATETIME_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Timestamp.h>
#include <CxxAbb/Timespan.h>
#include <CxxAbb/TimeZone.h>

#define LOCAL_TIMEZONE ":/etc/localtime"

namespace CxxAbb
{

/** @brief Timezone aware Date and Time representation. All times are in given timezone.
 * @see Use DateTime for timezone independent representation.
 * Conversion from timestamp to datetime representation
 * Offset from UTC for a given TimeZone is constant. The DST is only factor that change time to
 * time. By getting DST for given time in timezone we can get correct UTC offset.
 *
 * Algorithms are based on [http://mysite.verizon.net/aesir_research/date/date0.htm] by Peter Baum
 */
class CXXABB_API LocalDateTime
{
public:

	/** @brief The Julian Day Number, Julian Day, or JD of a particular instant of time
	 * is the number of days and fractions of a day since 12 hours Universal Time
	 * (Greenwich mean noon) on January 1 of the year -4712, where the year is given in the
	 * Julian proleptic calendar.
	 */
	typedef double JulianDay;

	enum Months
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December,

		MONTH_COUNT = 12
	};

	enum DaysOfWeek
	{
		Sunday = 0,
		Monday,
		Tuesday,
		Wenesday,
		Thursday,
		Friday,
		Saturday,

		DAYS_OF_WEEK_COUNT
	};

	static const unsigned MaxHours              = 23;
	static const unsigned HoursPerDay           = 24;
	static const unsigned MaxMinutes            = 59;
	static const unsigned MinutesPerHour        = 60;
	static const unsigned MinutesPerDay         = 1440;
	static const unsigned MaxSeconds            = 59;
	static const unsigned SecondsPerDay         = 86400;
	static const unsigned SecondsPerHour        = 3600;
	static const unsigned SecondsPerMinute      = 60;
	static const unsigned MSecsPerDay           = 86400000;
	static const unsigned MSecsPerHour          = 3600000;
	static const unsigned MSecsPerMinute        = 60000;
	static const unsigned MSecsPerSecond        = 1000;
	static const unsigned MaxMSecs              = 999;
	static const unsigned MaxMuSecs             = 999;
	static const CxxAbb::UInt64 MuSecsPerDay     = 86400000000U;
	static const CxxAbb::UInt64 MuSecsPerHour    = 3600000000U;
	static const unsigned MuSecsPerMinute       = 60000000;
	static const unsigned MuSecsPerSecond       = 1000000;
	static const unsigned MuSecsPerMSecond      = 1000;
	static const unsigned UtcUnitsPerMSec       = 10000; // 100 NanoSeconds
	static const unsigned UtcUnitsPerSecond     = 10000000; // 100 NanoSeconds
	static const unsigned UtcUnitsPerMinute     = 600000000; // 100 NanoSeconds
	static const CxxAbb::UInt64 UtcUnitsPerHour  = UtcUnitsPerMinute*60; // 100 NanoSeconds
	static const CxxAbb::UInt64 UtcUnitsPerDay   = 864000000000; // 100 NanoSeconds
	static const JulianDay JulianDayOfGregReform = 2299160.5; // JD of 1582-10-15
	static const JulianDay JulianDayOfGregDay0   = 1721118.5; // JD of Gregorian March 1 of Year 0

    static const unsigned DaysPerYear      = 365;
    static const unsigned DaysPerLeapYear  = 366;

    /** @brief Create LocalDateTime form current time instance
	 */
	LocalDateTime(const char * _zZone = LOCAL_TIMEZONE);

    /** @brief Create LocalDateTime form give DateTime
	 */
	LocalDateTime(DateTime & _dt, const char * _zZone = LOCAL_TIMEZONE);

    /** @brief Create LocalDateTime form give time instance
	 */
	LocalDateTime(CxxAbb::Timestamp _timestamp, const char * _zZone = LOCAL_TIMEZONE);

    /** @brief Create LocalDateTime form Epoch value
     * Ex: 1348722321 ~ Thu Sep 27 05:05:21 UTC 2012
	 */
	LocalDateTime(std::time_t _tEpochSecs, const char * _zZone = LOCAL_TIMEZONE);

    /** @brief Create LocalDateTime form Epoch in millisecond precision
     * Ex: 1348722321.096 ~ Thu Sep 27 05:05:21.096 UTC 2012
	 */
	LocalDateTime(double _dEpochMsecs, const char * _zZone = LOCAL_TIMEZONE);

	/** @brief Create LocalDateTime with Gregorian date times
	 */
	LocalDateTime(short _iYear, short _iMonth, short _iDay, short _iHour = 0,
		short _iMinute = 0, 	short _iSecond = 0, short _iMSecond = 0, short _iMuSecond = 0,
		const char * _zZone = LOCAL_TIMEZONE);

	LocalDateTime(Timestamp::UtcTimeVal _utcTime, Timestamp::TimeDiff _diff,
		const char * _zZone = LOCAL_TIMEZONE);

	LocalDateTime(const LocalDateTime & _rhs);

	~LocalDateTime();

	void Swap(LocalDateTime& _rhs);

	LocalDateTime & Set(short _iYear, short _iMonth, short _iDay, short _iHour = 0,
		short _iMinute = 0, short _iSecond = 0, short _iMSecond = 0, short _iMuSecond = 0,
		const char * _zZone = LOCAL_TIMEZONE);

	/// Getters

	/** @brief Get timezone name
	 *
	 */
	inline const char * ZoneName()
	{
		return m_ZoneInfo.GetZoneName();
	}

	/** @brief Get timezone difference from UTC in seconds
	 */
	inline int TimeZoneDiff()
	{
		return i_Tzd;
	}

	/** @brief Return Year [0-9999]
	 */
	short Year() const
	{
		return i_Year;
	}

	/** @brief Return Month [1-12]
	 */
	short Month() const
	{
		return i_Month;
	}

	/** @brief Return Day of Month [1-31]
	 */
	short Day() const
	{
		return i_Day;
	}

	/** @brief Return Hour of day in 24Hr [0-23]
	 */
	short Hour() const
	{
		return i_Hour;
	}

	/** @brief Return Minute of Hour [0-59]
	 */
	short Minute() const
	{
		return i_Minute;
	}

	/** @brief Return Second of minute [0-59]
	 */
	short Second() const
	{
		return i_Second;
	}

	/** @brief Return MilliSecond of second [0-999]
	 */
	short MilliSecond() const
	{
		return i_MSecond;
	}

	/** @brief Return MicroSecond of Millisecond [0-999]
	 */
	short MicroSecond() const
	{
		return i_MuSecond;
	}

	/** @brief Week Number of Year [0-53]
	 * Week number according to the ISO-8601 standard,
	 * weeks starting on Monday. The first week (1) of the year is the week that
	 * contains that year's first Thursday (='First 4-day week').
	 * The week 0 is the week that contains reset of the days.
	 * The highest week number in a year is either 52 or 53
	 */
	short Week(DaysOfWeek _eFirstDayOfWeek = Monday) const;

	/** @brief Return the julian date number [1-366]
	 */
	short DayOfYear() const;

	/** @brief Return the day nymber of week [0=Sunday to 6=Saturday]
	 */
	DaysOfWeek DayOfWeek() const;

	/** @brief Return Julian Day Number (JD)
	 *  Number of days and fractions of a day since 12 hours Universal Time (Greenwich mean noon)
	 *  on January 1 of the year -4712, where the year is given in the Julian proleptic calendar.
	 */
	JulianDay JulianDayNumber() const;

	/** @brief Return UtcTime value
	 * 100 nanosecond units since 1582-10-15 Midnight (Gregorian Calendar Start)
	 */
	Timestamp::UtcTimeVal UtcTime() const;

	/** @brief Timestamp of current LocalDateTime
	 */
	Timestamp TimestampValue() const;

	/** @brief Return UTC date time equivalent of current local time
	 */
	CxxAbb::DateTime UtcDateTime() const;

	/// Print functions
	std::string WeekDayName() const;
	std::string WeekDayShortName() const;
	std::string MonthName() const;
	std::string MonthShortName() const;

	/** @brief Add Musecs to the current date
	 *  negative also accepted
	 */
	void AddMicroSeconds(CxxAbb::Int64 _iMuSecs);

	/** @brief Add Milliseconds to the current date
	 *  negative also accepted
	 */
	void AddMilliSeonds(CxxAbb::Int64 _iMSecs);

	/** @brief Add Seconds to the current date
	 *  negative also accepted
	 */
	void AddSeconds(int _iSecs);

	/** @brief Add Minutes to the current date
	 *  negative also accepted
	 */
	void AddMinutes(int _iMins);

	/** @brief Add Hours to the current date
	 *  negative also accepted
	 */
	void AddHours(int _iHours);

	/** @brief Add days to the current date
	 *  negative days also accepted
	 */
	void AddDays(int _iDays);

	/** @brief Add weeks to current date
	 *  negative weeks also accepted
	 */
	void AddWeeks(int _iWeeks);

	/** @brief Add months to current date
	 *  negative months also accepted
	 */
	void AddMonths(int _iMonths);

	/// Public Static helper functions

	/** @brief Check if given year is Leap year
	 */
	static bool IsLeapYear(int _year);

	/** @brief Get how many days in given year-month
	 */
	static int DaysOfMonth(int _year, int _month);

	/** @brief Check if given date time is valid by checking range
	 */
	static bool IsValid(short _iYear, short _iMonth, short _iDay, short _iHour = 0,
		short _iMinute = 0, short _iSecond = 0, short _iMSecond = 0, short _iMuSecond = 0);

	LocalDateTime& operator = (const LocalDateTime& _rhs);
	LocalDateTime& operator = (const Timestamp& _rhs);
	LocalDateTime& operator = (LocalDateTime::JulianDay _jd);

	bool operator == (const LocalDateTime& _dateTime) const;
	bool operator != (const LocalDateTime& _dateTime) const;
	bool operator <  (const LocalDateTime& _dateTime) const;
	bool operator <= (const LocalDateTime& _dateTime) const;
	bool operator >  (const LocalDateTime& _dateTime) const;
	bool operator >= (const LocalDateTime& _dateTime) const;

	LocalDateTime  operator +  (const Timespan& _span) const;
	LocalDateTime  operator -  (const Timespan& _span) const;
	Timespan  operator -  (const LocalDateTime& _dateTime) const;
	LocalDateTime& operator += (const Timespan& _span);
	LocalDateTime& operator -= (const Timespan& _span);

protected:
	/// Internal Static helper functions
	static JulianDay Utc2Julian(CxxAbb::Timestamp::UtcTimeVal _utc);

	static JulianDay Gregorian2Julian(short _iYear, short _iMonth, short _iDay,
		short _iHour = 0, short _iMinute = 0, short _iSecond = 0, short _iMSecond = 0,
		short _iMuSecond = 0);
	static JulianDay Gregorian2Julian2(short _iYear, short _iMonth, short _iDay,
		short _iHour = 0, short _iMinute = 0, short _iSecond = 0, short _iMSecond = 0,
		short _iMuSecond = 0);

	static CxxAbb::Timestamp::UtcTimeVal Julian2Utc(JulianDay _jd);

	void Julian2Gregorian(JulianDay _jd);
	void Julian2Gregorian2(JulianDay _jd);

	void Utc2Time(CxxAbb::Timestamp::UtcTimeVal _utc);

private:
	void CorrectBounds(short& _lower, short& _upper, short _range);
	void Normalize();
	void Update();

	CxxAbb::TimeZoneInfo m_ZoneInfo;
	CxxAbb::Int16 i_Tzd;
	CxxAbb::Timestamp::UtcTimeVal t_UtcTime;
	CxxAbb::Int16 i_Year;
	CxxAbb::Int16 i_Month;
	CxxAbb::Int16 i_Day;
	CxxAbb::Int16 i_Hour;
	CxxAbb::Int16 i_Minute;
	CxxAbb::Int16 i_Second;
	CxxAbb::Int16 i_MSecond;
	CxxAbb::Int16 i_MuSecond;
};

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_LOCALDATETIME_H_ */

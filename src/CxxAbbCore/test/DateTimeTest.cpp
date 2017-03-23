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
 * DateTimeTest.cpp
 *
 * FileId      : $Id: DateTimeTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 12, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */

#include <CxxAbb/TimeZone.h>
#include <CxxAbb/Timestamp.h>
#include <CxxAbb/Timespan.h>
#include <CxxAbb/DateTime.h>
#include <CxxAbb/LocalDateTime.h>

#include <list>

#include <gtest/gtest.h>

using CxxAbb::Timestamp;
using CxxAbb::Timespan;
using CxxAbb::TimeZone;
using CxxAbb::TimeZoneInfo;
using CxxAbb::DateTime;

TEST(DateTimeTest, Timestamp)
{
	Timestamp ts;
	std::time_t t = time(NULL);

	ASSERT_TRUE(ts.Epoch() == t);

//	ts.Now();
//	sleep(1);
//	t = time(NULL);
//	ASSERT_FALSE(ts.Epoch() == t);

	Timestamp t1;
	usleep(5000); // make this platform independent
	Timestamp t2;
	Timestamp t3 = t2;
	ASSERT_TRUE (t1 != t2);
	ASSERT_TRUE (!(t1 == t2));
	ASSERT_TRUE (t2 > t1);
	ASSERT_TRUE (t2 >= t1);
	ASSERT_TRUE (!(t1 > t2));
	ASSERT_TRUE (!(t1 >= t2));
	ASSERT_TRUE (t2 == t3);
	ASSERT_TRUE (!(t2 != t3));
	ASSERT_TRUE (t2 >= t3);
	ASSERT_TRUE (t2 <= t3);
	Timestamp::TimeDiff d = (t2 - t1);
	//COUT_LOG() << "Diff : " << d;
	ASSERT_TRUE (d >= 4900 && d <= 7000);

	t1.Swap(t2);
	ASSERT_TRUE (t1 > t2);
	t2.Swap(t1);

	Timestamp::UtcTimeVal tv = t1.Utc();
	Timestamp t4 = Timestamp::FromUtc(tv);
	ASSERT_TRUE (t1 == t4);

	Timestamp epoch(0);
	tv = epoch.Utc();
	ASSERT_TRUE (tv >> 32 == 0x01B21DD2);
	ASSERT_TRUE ((tv & 0xFFFFFFFF) == 0x13814000);

	Timestamp now;
	usleep(201000);
	ASSERT_TRUE (now.Elapsed() >= 200000);
	ASSERT_TRUE (now.IsElapsed(200000));
	ASSERT_TRUE (!now.IsElapsed(2000000));

	/// timestamp performance test
//	for(int i=0; i<10; ++i)
//	{
//		Timestamp x;
//		//usleep(1);
//		Timestamp y;
//		COUT_LOG() << "Diff : " << (y - x);
//	}
}
 
TEST(DateTimeTest, TimespanCreation)
{
	Timespan ts;
	ASSERT_TRUE (ts.TotalMicroseconds() == 0);
	ts = Timespan::MuSecondsPerDay;
	ASSERT_TRUE (ts.TotalMicroseconds() == Timespan::MuSecondsPerDay);
	ASSERT_TRUE (ts.TotalMilliseconds() == 86400000);
	ASSERT_TRUE (ts.TotalSeconds() == 86400);
	ASSERT_TRUE (ts.TotalMinutes() == 60*24);
	ASSERT_TRUE (ts.TotalHours() == 24);

	ASSERT_TRUE (ts.Days() == 1);
	ASSERT_TRUE (ts.Microseconds() == 0);
	ASSERT_TRUE (ts.Milliseconds() == 0);
	ASSERT_TRUE (ts.Seconds() == 0);
	ASSERT_TRUE (ts.Minutes() == 0);
	ASSERT_TRUE (ts.Hours() == 0);

	ts.Set(2, 12, 30, 10, 123456);
	ASSERT_TRUE (ts.Microseconds() == 456);
	ASSERT_TRUE (ts.Milliseconds() == 123);
	ASSERT_TRUE (ts.Seconds() == 10);
	ASSERT_TRUE (ts.Minutes() == 30);
	ASSERT_TRUE (ts.Hours() == 12);
	ASSERT_TRUE (ts.Days() == 2);

	ts.Set(0, 36, 30, 10, 123456);
	ASSERT_TRUE (ts.Microseconds() == 456);
	ASSERT_TRUE (ts.Milliseconds() == 123);
	ASSERT_TRUE (ts.SecondsFraction() == 123456);
	ASSERT_TRUE (ts.Seconds() == 10);
	ASSERT_TRUE (ts.Minutes() == 30);
	ASSERT_TRUE (ts.Hours() == 12);
	ASSERT_TRUE (ts.Days() == 1);

	ts.Set(0, 0, 2190, 10, 123456);
	ASSERT_TRUE (ts.Microseconds() == 456);
	ASSERT_TRUE (ts.Milliseconds() == 123);
	ASSERT_TRUE (ts.SecondsFraction() == 123456);
	ASSERT_TRUE (ts.Seconds() == 10);
	ASSERT_TRUE (ts.Minutes() == 30);
	ASSERT_TRUE (ts.Hours() == 12);
	ASSERT_TRUE (ts.Days() == 1);
}

TEST(DateTimeTest, TimespanOperators)
{
	Timespan ts1(10000000);
	Timespan ts2(20000000);
	Timespan ts3(20000000);

	ASSERT_TRUE (ts1 != ts2);
	ASSERT_TRUE (!(ts1 == ts2));
	ASSERT_TRUE (ts1 <= ts2);
	ASSERT_TRUE (ts1 < ts2);
	ASSERT_TRUE (ts2 > ts1);
	ASSERT_TRUE (ts2 >= ts1);

	ASSERT_TRUE (ts2 == ts3);
	ASSERT_TRUE (!(ts2 != ts3));
	ASSERT_TRUE (ts2 >= ts3);
	ASSERT_TRUE (ts2 <= ts3);
	ASSERT_TRUE (!(ts2 > ts3));
	ASSERT_TRUE (!(ts2 < ts3));

	ASSERT_TRUE (ts1 == 10000000);
	ASSERT_TRUE (ts1 != 20000000);
	ASSERT_TRUE (ts1 <= 10000000);
	ASSERT_TRUE (ts1 <= 20000000);
	ASSERT_TRUE (ts1 >= 10000000);
	ASSERT_TRUE (ts1 >= 5000000);
	ASSERT_TRUE (ts1 < 20000000);
	ASSERT_TRUE (ts1 > 5000000);

	ts1 = Timespan(100000000);
	ts2 = Timespan(50000000);
	ts3 = Timespan();
	ts3 = ts1 + ts2;
	ASSERT_TRUE (ts3 == 150000000);
	ts3 = ts1 + 30000000;
	ASSERT_TRUE (ts3 == 130000000);
	ts3 = ts1 - ts2;
	ASSERT_TRUE (ts3 == 50000000);
	ts3 = ts1 - 20000000;
	ASSERT_TRUE (ts3 == 80000000);
	ts3 += 20000000;
	ASSERT_TRUE (ts3 == ts1);
	ts3 -= ts2;
	ASSERT_TRUE (ts3 == ts2);

	ts1 = Timespan(10000000);
	ts2 = Timespan(50000000);

	ASSERT_TRUE (ts1 < ts2);
	ts1.Swap(ts2);
	ASSERT_TRUE (ts2 < ts1);
}

namespace {
	void CheckTimeZones(std::string & _sZone, std::time_t _t, int _defOffset)
	{
		TimeZoneInfo zi(_sZone.c_str());
		int iDiff = zi.GetTimeZoneDiff(_t);
		bool dst = zi.IsDST(_t);
		//COUT_LOG() << "TimeZone " << zi.GetZoneName() << " Dst=" << dst <<  " diff=" << iDiff;
		if(!dst)
		{
			ASSERT_EQ(iDiff, _defOffset);
		}
		else
		{
			ASSERT_EQ(iDiff, (_defOffset + 3600));
		}
	}
}

TEST(DateTimeTest, TimeZone)
{
	std::string name = TimeZone::TimeZoneName();
	std::string stdName = TimeZone::TimeZoneNameStandard();
	std::string dstName = TimeZone::TimeZoneNameDst();
	COUT_LOG() << "Timezone Names: Cur=" << name << ", Std=" << stdName << ", Dst=" << dstName ;
	int utcOffset = TimeZone::UtcOffset();
	COUT_LOG() << "UTC Offset: " << utcOffset;
	int dst = TimeZone::DstCorrection();
	COUT_LOG() << "DST Offset: " << dst;

	typedef std::list<std::pair<std::string, int> > ZoneList;
	ZoneList lst_Zones;
	lst_Zones.push_back(std::make_pair("EST",-18000)); // fixed timezone, no DST applies
	lst_Zones.push_back(std::make_pair("EST5EDT",-18000));
	lst_Zones.push_back(std::make_pair("Africa/Cairo",7200)); // +2 EET
	lst_Zones.push_back(std::make_pair("Africa/Casablanca",0)); // 0 WET
	lst_Zones.push_back(std::make_pair("Africa/Tripoli",7200)); // +2 EET
	lst_Zones.push_back(std::make_pair("Africa/Tunis",3600)); // +1 CET
	lst_Zones.push_back(std::make_pair("America/New_York",-18000)); // -5 EST/EDT
	lst_Zones.push_back(std::make_pair("Asia/Almaty",21600)); // +6
	lst_Zones.push_back(std::make_pair("Asia/Amman",7200)); // +2
	lst_Zones.push_back(std::make_pair("Asia/Bahrain",10800)); // +3
	lst_Zones.push_back(std::make_pair("Asia/Beirut",7200)); // + 2
	lst_Zones.push_back(std::make_pair("Asia/Calcutta",19800)); // +5.30
	lst_Zones.push_back(std::make_pair("Asia/Colombo", 19800)); // +5.30
	lst_Zones.push_back(std::make_pair("Asia/Dubai",14400)); // +4
	lst_Zones.push_back(std::make_pair("Asia/Jakarta",25200)); // +7
	lst_Zones.push_back(std::make_pair("Asia/Karachi",18000)); // +5
	lst_Zones.push_back(std::make_pair("Asia/Kuala_Lumpur",28800)); // +8
	lst_Zones.push_back(std::make_pair("Asia/Kuwait",10800)); // +3
	lst_Zones.push_back(std::make_pair("Asia/Muscat",14400)); // +4
	lst_Zones.push_back(std::make_pair("Asia/Singapore",28800)); // +8
	lst_Zones.push_back(std::make_pair("Asia/Tehran",12600)); // +3.30
	lst_Zones.push_back(std::make_pair("Europe/Kiev",7200)); // +2
	lst_Zones.push_back(std::make_pair("Europe/London",0)); // 0
	lst_Zones.push_back(std::make_pair("Europe/Moscow",14400)); // +4

	TimeZoneInfo zi;
	std::time_t t = time(NULL);
	int iDiff = zi.GetTimeZoneDiff(t);
	COUT_LOG() << "TimeZone diff : " << zi.GetZoneName() << "=" << iDiff;

	ZoneList::iterator ite = lst_Zones.begin();
	while(ite != lst_Zones.end())
	{
		CheckTimeZones(ite->first, time(NULL), ite->second);
		++ite;
	}

}

// ctor and set/get
TEST(DateTimeTest, DateTimeCreation)
{
	DateTime d;
	time_t t = time(NULL);
	ASSERT_EQ(d.TimestampValue().Epoch(), t);


	Timestamp ts(0); // Unix epoch 1970-01-01 00:00:00 Thursday
	DateTime dt(ts);
	ASSERT_TRUE (dt.Year() == 1970);
	ASSERT_TRUE (dt.Month() == 1);
	ASSERT_TRUE (dt.Day() == 1);
	ASSERT_TRUE (dt.Hour() == 0);
	ASSERT_TRUE (dt.Minute() == 0);
	ASSERT_TRUE (dt.Second() == 0);
	ASSERT_TRUE (dt.MilliSecond() == 0);
	ASSERT_TRUE (dt.DayOfWeek() == DateTime::Thursday);
	ASSERT_TRUE (dt.JulianDayNumber() == 2440587.5);
	ASSERT_TRUE (dt.TimestampValue() == 0);

	ts = Timestamp::FromEpoch(1000000000);
	dt = ts; // 2001-09-09 01:46:40 Sunday
	ASSERT_TRUE (dt.Year() == 2001);
	ASSERT_TRUE (dt.Month() == 9);
	ASSERT_TRUE (dt.Day() == 9);
	ASSERT_TRUE (dt.Hour() == 1);
	ASSERT_TRUE (dt.Minute() == 46);
	ASSERT_TRUE (dt.Second() == 40);
	ASSERT_TRUE (dt.MilliSecond() == 0);
	ASSERT_TRUE (dt.DayOfWeek() == 0);
	ASSERT_TRUE (dt.TimestampValue().Epoch() == 1000000000);
	EXPECT_FLOAT_EQ(dt.JulianDayNumber(), 2452161.574074);

	// Test that we can represent down to the microsecond.
	dt = DateTime(2010, 1, 31, 17, 30, 15, 800, 3);

	ASSERT_TRUE (dt.Year() == 2010);
	ASSERT_TRUE (dt.Month() == 1);
	ASSERT_TRUE (dt.Day() == 31);
	ASSERT_TRUE (dt.Hour() == 17);
	ASSERT_TRUE (dt.Minute() == 30);
	ASSERT_TRUE (dt.Second() == 15);
	ASSERT_TRUE (dt.MilliSecond() == 800);
	ASSERT_TRUE (dt.MicroSecond() == 3);

	dt = DateTime(1970, 1, 1);
	ASSERT_TRUE (dt.Year() == 1970);
	ASSERT_TRUE (dt.Month() == 1);
	ASSERT_TRUE (dt.Day() == 1);
	ASSERT_TRUE (dt.Hour() == 0);
	ASSERT_TRUE (dt.Minute() == 0);
	ASSERT_TRUE (dt.Second() == 0);
	ASSERT_TRUE (dt.MilliSecond() == 0);
	ASSERT_TRUE (dt.DayOfWeek() == 4);
	ASSERT_TRUE (dt.JulianDayNumber() == 2440587.5);
	ASSERT_TRUE (dt.TimestampValue() == 0);

	dt.Set(2001, 9, 9, 1, 46, 40);
	ASSERT_TRUE (dt.Year() == 2001);
	ASSERT_TRUE (dt.Month() == 9);
	ASSERT_TRUE (dt.Day() == 9);
	ASSERT_TRUE (dt.Hour() == 1);
	ASSERT_TRUE (dt.Minute() == 46);
	ASSERT_TRUE (dt.Second() == 40);
	ASSERT_TRUE (dt.MilliSecond() == 0);
	ASSERT_TRUE (dt.DayOfWeek() == 0);
	ASSERT_TRUE (dt.TimestampValue().Epoch() == 1000000000);
	EXPECT_FLOAT_EQ (dt.JulianDayNumber(), 2452161.574074);
}

TEST(DateTimeTest, Conversions)
{
	DateTime dt1(2005, 1, 28, 14, 24, 44, 234);
	Timestamp ts1 = dt1.TimestampValue();
	DateTime dt2(ts1);
	Timestamp ts2 = dt2.TimestampValue();
	DateTime dt3;
	dt3 = dt1;
	Timestamp ts3 = dt3.TimestampValue();
	DateTime dt4(dt2);
	Timestamp ts4 = dt4.TimestampValue();

	ASSERT_TRUE (ts1 == ts2);
	ASSERT_TRUE (ts2 == ts3);
	ASSERT_TRUE (ts3 == ts4);

	ASSERT_TRUE (dt2.Year() == 2005);
	ASSERT_TRUE (dt2.Month() == 1);
	ASSERT_TRUE (dt2.Day() == 28);
	ASSERT_TRUE (dt2.Hour() == 14);
	ASSERT_TRUE (dt2.Minute() == 24);
	ASSERT_TRUE (dt2.Second() == 44);
	ASSERT_TRUE (dt2.MilliSecond() == 234);
	ASSERT_TRUE (dt2.DayOfWeek() == 5);
}

TEST(DateTimeTest, StaticFunctions)
{
	ASSERT_TRUE (DateTime::IsLeapYear(1984));
	ASSERT_TRUE (DateTime::IsLeapYear(1988));
	ASSERT_TRUE (DateTime::IsLeapYear(1992));
	ASSERT_TRUE (DateTime::IsLeapYear(1996));
	ASSERT_TRUE (DateTime::IsLeapYear(2000));
	ASSERT_TRUE (DateTime::IsLeapYear(2400));
	ASSERT_TRUE (!DateTime::IsLeapYear(1995));
	ASSERT_TRUE (!DateTime::IsLeapYear(1998));
	ASSERT_TRUE (!DateTime::IsLeapYear(2001));
	ASSERT_TRUE (!DateTime::IsLeapYear(1800));
	ASSERT_TRUE (!DateTime::IsLeapYear(1900));

	ASSERT_TRUE (DateTime::DaysOfMonth(2000, 1) == 31);
	ASSERT_TRUE (DateTime::DaysOfMonth(2000, 2) == 29);
	ASSERT_TRUE (DateTime::DaysOfMonth(1999, 2) == 28);

	ASSERT_TRUE (DateTime::IsValid(2000,12,31));
	ASSERT_FALSE (DateTime::IsValid(2000,12,31, 25,10,5,123,123));
}

TEST(DateTimeTest, CalculatedData)
{
	DateTime dt1(2005, 1, 1);
	ASSERT_TRUE (dt1.DayOfYear() == 1);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 0);
	dt1.Set(2005, 1, 3);
	ASSERT_TRUE (dt1.DayOfYear() == 3);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 1);
	dt1.Set(2005, 1, 9);
	ASSERT_TRUE (dt1.DayOfYear() == 9);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 1);
	dt1.Set(2005, 1, 10);
	ASSERT_TRUE (dt1.DayOfYear() == 10);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 2);
	dt1.Set(2005, 2, 1);
	ASSERT_TRUE (dt1.DayOfYear() == 32);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 5);
	dt1.Set(2005, 12, 31);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 52);
	dt1.Set(2007, 1, 1);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 1);
	dt1.Set(2007, 12, 31);
	ASSERT_TRUE (dt1.Week(DateTime::Monday) == 53);

	// Jan 1 is Mon
	dt1.Set(2001, 1, 1);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2001, 1, 7);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2001, 1, 8);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(2001, 1, 21);
	ASSERT_TRUE (dt1.Week() == 3);
	dt1.Set(2001, 1, 22);
	ASSERT_TRUE (dt1.Week() == 4);

	// Jan 1 is Tue
	dt1.Set(2002, 1, 1);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2002, 1, 6);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2002, 1, 7);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(2002, 1, 20);
	ASSERT_TRUE (dt1.Week() == 3);
	dt1.Set(2002, 1, 21);
	ASSERT_TRUE (dt1.Week() == 4);

	// Jan 1 is Wed
	dt1.Set(2003, 1, 1);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2003, 1, 5);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2003, 1, 6);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(2003, 1, 19);
	ASSERT_TRUE (dt1.Week() == 3);
	dt1.Set(2003, 1, 20);
	ASSERT_TRUE (dt1.Week() == 4);

	// Jan 1 is Thu
	dt1.Set(2004, 1, 1);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2004, 1, 4);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2004, 1, 5);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(2004, 1, 18);
	ASSERT_TRUE (dt1.Week() == 3);
	dt1.Set(2004, 1, 19);
	ASSERT_TRUE (dt1.Week() == 4);

	// Jan 1 is Fri
	dt1.Set(1999, 1, 1);
	ASSERT_TRUE (dt1.Week() == 0);
	dt1.Set(1999, 1, 3);
	ASSERT_TRUE (dt1.Week() == 0);
	dt1.Set(1999, 1, 4);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(1999, 1, 17);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(1999, 1, 18);
	ASSERT_TRUE (dt1.Week() == 3);

	// Jan 1 is Sat
	dt1.Set(2000, 1, 1);
	ASSERT_TRUE (dt1.Week() == 0);
	dt1.Set(2000, 1, 2);
	ASSERT_TRUE (dt1.Week() == 0);
	dt1.Set(2000, 1, 3);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(2000, 1, 16);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(2000, 1, 17);
	ASSERT_TRUE (dt1.Week() == 3);

	// Jan 1 is Sun
	dt1.Set(1995, 1, 1);
	ASSERT_TRUE (dt1.Week() == 0);
	dt1.Set(1995, 1, 2);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(1995, 1, 3);
	ASSERT_TRUE (dt1.Week() == 1);
	dt1.Set(1995, 1, 15);
	ASSERT_TRUE (dt1.Week() == 2);
	dt1.Set(1995, 1, 16);
	ASSERT_TRUE (dt1.Week() == 3);
}

// change vlaues (add/operators)
TEST(DateTimeTest, Operators1)
{
    DateTime dt1(2005, 1, 1, 0, 15, 30);
    DateTime dt2(2005, 1, 2, 0, 15, 30);
    DateTime dt3(dt1);

    ASSERT_TRUE (dt1 < dt2);
    ASSERT_TRUE (dt1 <= dt2);
    ASSERT_TRUE (dt2 > dt1);
    ASSERT_TRUE (dt2 >= dt1);
    ASSERT_TRUE (dt1 != dt2);
    ASSERT_TRUE (!(dt1 == dt2));

    ASSERT_TRUE (dt1 == dt3);
    ASSERT_TRUE (!(dt1 != dt3));
    ASSERT_TRUE (dt1 >= dt3);
    ASSERT_TRUE (dt1 <= dt3);
    ASSERT_TRUE (!(dt1 > dt3));
    ASSERT_TRUE (!(dt1 < dt3));

    static const struct
	{
		int year;
		int month;
		int day;
	} values[] =
	{
		{    1,  1,  1 },
		{   10,  4,  5 },
		{  100,  6,  7 },
		{ 1000,  8,  9 },
		{ 2000,  1, 31 },
		{ 2002,  7,  4 },
		{ 2002, 12, 31 },
		{ 2003,  1,  1 },
		{ 2003,  1,  2 },
		{ 2003,  8,  5 },
		{ 2003,  8,  6 },
		{ 2003,  8,  7 },
		{ 2004,  9,  3 },
		{ 2004,  9,  4 },
	};

	const int num_values = sizeof values / sizeof *values;
	for (int i = 0; i < num_values; ++i)
	{
		DateTime v;
		const DateTime& V = v;
		v.Set(values[i].year, values[i].month, values[i].day);
		for (int j = 0; j < num_values; ++j)
		{
			DateTime u;
			const DateTime& U = u;
			u.Set(values[j].year, values[j].month, values[j].day);

			ASSERT_EQ((j < i), (U < V));
			ASSERT_EQ((j <= i), (U <= V));
			ASSERT_EQ((j > i), (U > V));
			ASSERT_EQ((j >= i), (U >= V));
		}
	}

}

TEST(DateTimeTest, Operators2)
{

	DateTime dt1(2005, 1, 1, 0, 15, 30);
	DateTime dt2(2005, 1, 2, 0, 15, 30);

    Timespan s = dt2 - dt1;
    ASSERT_EQ (s.Days(), 1);

    DateTime dt3 = dt1 + s;
    ASSERT_EQ (dt3, dt2);

    dt3 -= s;
    ASSERT_EQ (dt3, dt1);
    dt1 += s;
    ASSERT_EQ (dt1, dt2);

    static const struct
	{
		int lineNum;		// source line number
		int year1;			// operand/result date1 year
		int month1;			// operand/result date1 month
		unsigned int day1;	// operand/result date1 day
		int numDays;		// operand/result 'int' number of days
		int year2;			// operand/result date2 year
		int month2;			// operand/result date2 month
		unsigned int day2;	// operand/result date2 day
	} data[] =
	{
		//          - - - -first- - - -           - - - second - - -
		//line no.  year   month   day   numDays  year   month   day
		//-------   -----  -----  -----  -------  -----  -----  -----
		{ __LINE__,       1,     1,     1,      1,      1,     1,     2 },
		{ __LINE__,      10,     2,    28,      1,     10,     3,     1 },
		{ __LINE__,     100,     3,    31,      2,    100,     4,     2 },
		{ __LINE__,    1000,     4,    30,      4,   1000,     5,     4 },
		{ __LINE__,    1000,     6,     1,    -31,   1000,     5,     1 },
		{ __LINE__,    1001,     1,     1,   -365,   1000,     1,     1 },
		{ __LINE__,    1100,     5,    31,     30,   1100,     6,    30 },
		{ __LINE__,    1200,     6,    30,     32,   1200,     8,     1 },
		{ __LINE__,    1996,     2,    28,    367,   1997,     3,     1 },
		{ __LINE__,    1997,     2,    28,    366,   1998,     3,     1 },
		{ __LINE__,    1998,     2,    28,    365,   1999,     2,    28 },
		{ __LINE__,    1999,     2,    28,    364,   2000,     2,    27 },
		{ __LINE__,    1999,     2,    28,   1096,   2002,     2,    28 },
		{ __LINE__,    2002,     2,    28,  -1096,   1999,     2,    28 },
	};

	const int num_data = sizeof data / sizeof *data;
	for (int di = 0; di < num_data; ++di)
	{
		const int num_days = data[di].numDays;
		DateTime x(data[di].year1, data[di].month1, data[di].day1);
		const DateTime& X = x;
		//COUT_LOG() << "Line : " << data[di].lineNum << " Date: " << x.Year() << "-" << x.Month()
		//	<< "-" << x.Day() << " NumDays: " << num_days;
		//Timespan t(num_days, 0, 0, 0, 0);
		//COUT_LOG() << "Timespan : " << t.TotalMicroseconds();
		//x += t;
		x += Timespan(num_days, 0, 0, 0, 0);
		ASSERT_EQ(data[di].year2, X.Year());
		ASSERT_EQ(data[di].month2, X.Month());
		ASSERT_TRUE(data[di].day2 == (uint)X.Day());
	}
}

TEST(DateTimeTest, Operators3)
{
	static const struct
	{
		int lineNum;		// source line number
		int year1;			// (first) date year
		int month1;			// (first) date month
		unsigned int day1;	// (first) date day
		int year2;			// (second) date year
		int month2;			// (second) date month
		unsigned int day2;	// (second) date day
	} data[] =
	{
	   //          - - - -first- - - -    - - - second - - -
	   //line no.  year   month   day     year   month   day
	   //-------   -----  -----  -----    -----  -----  -----
	  { __LINE__,       1,     1,     1,       1,     1,     2 },
	  { __LINE__,      10,     2,    28,      10,     3,     1 },
	  { __LINE__,     100,     3,    31,     100,     4,     1 },
	  { __LINE__,    1000,     4,    30,    1000,     5,     1 },
	  { __LINE__,    1100,     5,    31,    1100,     6,     1 },
	  { __LINE__,    1200,     6,    30,    1200,     7,     1 },
	  { __LINE__,    1300,     7,    31,    1300,     8,     1 },
	  { __LINE__,    1400,     8,    31,    1400,     9,     1 },
	  { __LINE__,    1500,     9,    30,    1500,    10,     1 },
	  { __LINE__,    1600,    10,    31,    1600,    11,     1 },
	  { __LINE__,    1700,    11,    30,    1700,    12,     1 },
	  { __LINE__,    1800,    12,    31,    1801,     1,     1 },
	  { __LINE__,    1996,     2,    28,    1996,     2,    29 },
	  { __LINE__,    1997,     2,    28,    1997,     3,     1 },
	  { __LINE__,    1998,     2,    28,    1998,     3,     1 },
	  { __LINE__,    1999,     2,    28,    1999,     3,     1 },
	  { __LINE__,    2000,     2,    28,    2000,     2,    29 },
	  { __LINE__,    2001,     2,    28,    2001,     3,     1 },
	  { __LINE__,    2004,     2,    28,    2004,     2,    29 },
	  { __LINE__,    2100,     2,    28,    2100,     3,     1 },
	  { __LINE__,    2400,     2,    28,    2400,     2,    29 },
	};

	const int num_data = sizeof data / sizeof *data;
	int di;

	for (di = 0; di < num_data; ++di)
	{
		//const int line = data[di].lineNum;
		DateTime x = DateTime(data[di].year1, data[di].month1,
							  data[di].day1);
		// Would do pre-increment of x here.
		const DateTime& X = x;
		x = x + Timespan(1,0,0,0,0);
		DateTime y = x; const DateTime& Y = y;

		ASSERT_TRUE(data[di].year2  == X.Year());
		ASSERT_TRUE(data[di].month2 == X.Month());
		ASSERT_EQ(data[di].day2, (uint)X.Day());

		ASSERT_TRUE(data[di].year2  == Y.Year());
		ASSERT_TRUE(data[di].month2 == Y.Month());
		ASSERT_EQ(data[di].day2, (uint)Y.Day());
	}

	for (di = 0; di < num_data; ++di)
	{
		//const int line = data[di].lineNum;
		DateTime x = DateTime(data[di].year1, data[di].month1, data[di].day1);
		DateTime x1 = DateTime(data[di].year1, data[di].month1, data[di].day1);
		DateTime x2 = DateTime(data[di].year2, data[di].month2, data[di].day2);
		DateTime y = x; const DateTime& Y = y;

		// Would do post increment of x here.
		const DateTime& X = x;
		x = x + Timespan(1,0,0,0,0);

		ASSERT_TRUE(data[di].year2  == X.Year());
		ASSERT_TRUE(data[di].month2 == X.Month());
		ASSERT_EQ(data[di].day2, (uint)X.Day());

		ASSERT_TRUE(data[di].year1  == Y.Year());
		ASSERT_TRUE(data[di].month1 == Y.Month());
		ASSERT_EQ(data[di].day1, (uint)Y.Day());
	}

	for (di = 0; di < num_data; ++di)
	{
		//const int line = data[di].lineNum;
		DateTime x = DateTime(data[di].year2, data[di].month2, data[di].day2);
		const DateTime& X = x;
		x = x - Timespan(1,0,0,0,0);
		DateTime y = x; DateTime Y = y;

		ASSERT_TRUE(data[di].year1  == X.Year());
		ASSERT_TRUE(data[di].month1 == X.Month());
		ASSERT_EQ(data[di].day1, (uint)X.Day());

		ASSERT_TRUE(data[di].year1  == Y.Year());
		ASSERT_TRUE(data[di].month1 == Y.Month());
		ASSERT_EQ(data[di].day1, (uint)Y.Day());
	}

	for (di = 0; di < num_data; ++di)
	{
		//const int line = data[di].lineNum;
		DateTime x1 = DateTime(data[di].year1, data[di].month1, data[di].day1);
		DateTime x = DateTime(data[di].year2, data[di].month2, data[di].day2);
		DateTime y = x; DateTime Y = y;
		const DateTime& X = x;
		// would post-decrement x here.
		x = x - Timespan(1,0,0,0,0);

		ASSERT_TRUE(data[di].year1  == X.Year());
		ASSERT_TRUE(data[di].month1 == X.Month());
		ASSERT_EQ(data[di].day1, (uint)X.Day());

		ASSERT_TRUE(data[di].year2  == Y.Year());
		ASSERT_TRUE(data[di].month2 == Y.Month());
		ASSERT_EQ(data[di].day2 , (uint)Y.Day());
	}
}

TEST(DateTimeTest, Swap)
{
	DateTime dt1(2005, 1, 1, 0, 15, 30);
	DateTime dt2(2005, 1, 2, 0, 15, 30);
	DateTime dt3(2005, 1, 1, 0, 15, 30);
	DateTime dt4(2005, 1, 2, 0, 15, 30);

	dt1.Swap(dt2);
	ASSERT_TRUE (dt2 == dt3);
	ASSERT_TRUE (dt1 == dt4);
}

TEST(DateTimeTest, Usage)
{
	DateTime dt1(1776, 7, 4);
	ASSERT_TRUE (dt1.Year() == 1776);
	ASSERT_TRUE (dt1.Month() == 7);
	ASSERT_TRUE (dt1.Day() == 4);

	DateTime dt2(dt1);
	dt2 += Timespan(6, 0, 0, 0, 0);
	ASSERT_TRUE (dt2.Year() == 1776);
	ASSERT_TRUE (dt2.Month() == 7);
	ASSERT_TRUE (dt2.Day() == 10);

	Timespan span = dt2 - dt1;
	ASSERT_TRUE (span.Days() == 6);

	dt2 = DateTime(2012, 11, 1);
	dt2.AddDays(7);
	ASSERT_TRUE(dt2.Year() == 2012);
	ASSERT_TRUE(dt2.Month() == 11);
	ASSERT_TRUE(dt2.Day() == 8);

    static const struct
    {
        int d_lineNum;			// source line number
        int d_year;				// year under test
        unsigned int d_day;		// day-of-year under test
        int d_expMonth;			// expected month
        unsigned int d_expDay;	// expected day
    } data[] =
    {
		//line no.  year   dayOfYr     exp. month   exp. day
		//-------   -----  -------     ----------   --------
		{ __LINE__,          1,       1,          1,         1 },
		{ __LINE__,          1,       2,          1,         2 },
		{ __LINE__,          1,     365,         12,        31 },
		{ __LINE__,       1996,       1,          1,         1 },
		{ __LINE__,       1996,       2,          1,         2 },
		{ __LINE__,       1996,     365,         12,        30 },
		{ __LINE__,       1996,     366,         12,        31 }
    };

    const int num_data = sizeof data / sizeof *data;
    for (int di = 0; di < num_data; ++di)
    {
        //const int line      = data[di].d_lineNum;
        const int year      = data[di].d_year;
        const unsigned int day = data[di].d_day;

        const int exp_month = data[di].d_expMonth;
        const unsigned int exp_day   = data[di].d_expDay;
        const DateTime r(year, exp_month, exp_day);
        DateTime x(year, 1 ,1);
        const DateTime& X = x;

        x.AddDays(day - 1);
        ASSERT_TRUE(x == r);
        ASSERT_EQ(day, (uint)X.DayOfYear());
    }
}

TEST(DateTimeTest, Validate)
{
	static const struct
	{
		int  d_lineNum;  // source line number
		int  d_year;     // year under test
		int  d_month;    // month under test
		int  d_day;      // day under test
		bool d_exp;      // expected value
	} data[] =
	{
		//line no.  year   month   day     expected value
		//-------   -----  -----  -----    --------------
		{ __LINE__,    0,     0,     0,      false },
		{ __LINE__,    1,     1,     0,      false },
		{ __LINE__,    1,     0,     1,      false },
		{ __LINE__,    0,     1,     1,      true },
		{ __LINE__,    1,     1,    -1,      false },
		{ __LINE__,    1,    -1,     1,      false },
		{ __LINE__, 2004,     1,    32,      false },
		{ __LINE__, 2004,     2,    30,      false },
		{ __LINE__, 2004,     3,    32,      false },
		{ __LINE__, 2004,     4,    31,      false },
		{ __LINE__, 2004,     5,    32,      false },
		{ __LINE__, 2004,     6,    31,      false },
		{ __LINE__, 2004,     7,    32,      false },
		{ __LINE__, 2004,     8,    32,      false },
		{ __LINE__, 2004,     9,    31,      false },
		{ __LINE__, 2004,    10,    32,      false },
		{ __LINE__, 2004,    11,    31,      false },
		{ __LINE__, 2004,    12,    32,      false },
		{ __LINE__,    0,    12,    31,      true },
		{ __LINE__,    0,     2,    29,      true },
		{ __LINE__,    1,     1,     1,      true },
		{ __LINE__, 2010,     1,     2,      true },
		{ __LINE__, 2011,     2,     5,      true },
		{ __LINE__, 2012,     3,    10,      true },
		{ __LINE__, 2013,     4,    17,      true },
		{ __LINE__, 2014,     5,    23,      true },
		{ __LINE__, 1600,     2,    29,      true },
		{ __LINE__, 1700,     2,    29,      false },
		{ __LINE__, 1800,     2,    29,      false },
		{ __LINE__, 1900,     2,    29,      false },
		{ __LINE__, 2000,     2,    29,      true },
		{ __LINE__, 2100,     2,    29,      false },
	};

	const int num_data = sizeof data / sizeof *data;
	for (int di = 0; di < num_data; ++di)
	{
		//const int  line  = data[di].d_lineNum;
		const int  year  = data[di].d_year;
		const int  month = data[di].d_month;
		const int  day   = data[di].d_day;
		const bool exp   = data[di].d_exp;

		bool isValid = DateTime::IsValid(year, month, day);
		ASSERT_TRUE(exp == isValid);
	}
}

TEST(DateTimeTest, DayOfWeek)
{
	typedef DateTime::DaysOfWeek DOW;

	static const struct
	{
		int d_lineNum;	// source line number
		int d_year;		// year under test
		int d_month;	// month under test
		int d_day;		// day under test
		DOW d_expDay;	// number of days to be added
	} data[] =
	{
		//Line no.    year   month   day    expDay
		//-------   -----  -----  -----   -------
		{ __LINE__,  1600,   1,       1,   DateTime::Saturday  },
		{ __LINE__,  1600,   1,       2,   DateTime::Sunday    },
		{ __LINE__,  1600,   1,       3,   DateTime::Monday    },
		{ __LINE__,  1600,   1,       4,   DateTime::Tuesday   },
		{ __LINE__,  1600,   1,       5,   DateTime::Wednesday },
		{ __LINE__,  1600,   1,       6,   DateTime::Thursday  },
		{ __LINE__,  1600,   1,       7,   DateTime::Friday    },
		{ __LINE__,  1600,   1,       8,   DateTime::Saturday  },
		{ __LINE__,  1752,   8,      27,   DateTime::Sunday    },
		{ __LINE__,  1752,   8,      28,   DateTime::Monday    },
		{ __LINE__,  1752,   8,      29,   DateTime::Tuesday   },
		{ __LINE__,  1752,   8,      30,   DateTime::Wednesday },
		{ __LINE__,  1752,   8,      31,   DateTime::Thursday  },
		{ __LINE__,  1752,   9,       1,   DateTime::Friday    },
		{ __LINE__,  1752,   9,       2,   DateTime::Saturday  },
		{ __LINE__,  1752,   9,      14,   DateTime::Thursday  },
		{ __LINE__,  1752,   9,      15,   DateTime::Friday    },
		{ __LINE__,  1752,   9,      16,   DateTime::Saturday  },
		{ __LINE__,  1752,   9,      17,   DateTime::Sunday    },
		{ __LINE__,  1752,   9,      18,   DateTime::Monday    },
		{ __LINE__,  1752,   9,      19,   DateTime::Tuesday   },
		{ __LINE__,  1999,  12,      28,   DateTime::Tuesday   },
		{ __LINE__,  1999,  12,      29,   DateTime::Wednesday },
		{ __LINE__,  1999,  12,      30,   DateTime::Thursday  },
		{ __LINE__,  1999,  12,      31,   DateTime::Friday    },
		{ __LINE__,  2000,   1,       1,   DateTime::Saturday  },
		{ __LINE__,  2000,   1,       2,   DateTime::Sunday    },
		{ __LINE__,  2000,   1,       3,   DateTime::Monday    },
		{ __LINE__,  2000,   1,       4,   DateTime::Tuesday   },
	};

	const int num_data = sizeof data / sizeof *data;
	for (int di = 0; di < num_data ; ++di)
	{
		//const int line = data[di].d_lineNum;
		DateTime x = DateTime(data[di].d_year, data[di].d_month, data[di].d_day);
		const DateTime& X = x;
		ASSERT_TRUE( data[di].d_expDay == X.DayOfWeek());
	}
}

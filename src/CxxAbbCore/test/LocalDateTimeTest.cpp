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
 * LocalDateTimeTest.cpp
 *
 * FileId      : $Id: LocalDateTimeTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 26, 2012
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
using CxxAbb::LocalDateTime;

TEST(LocalDateTimeTest, Ctor)
{
	LocalDateTime dt(1970, 1, 1);

	ASSERT_TRUE (dt.Year() == 1970);
	ASSERT_TRUE (dt.Month() == 1);
	ASSERT_TRUE (dt.Day() == 1);
	ASSERT_TRUE (dt.Hour() == 0);
	ASSERT_TRUE (dt.Minute() == 0);
	ASSERT_TRUE (dt.Second() == 0);
	ASSERT_TRUE (dt.MilliSecond() == 0);
	ASSERT_TRUE (dt.DayOfWeek() == 4);
	// REMOVED: this fails when the current DST offset differs from
	//          the one on 1970-1-1
	//ASSERT_TRUE (dt.tzd() == Timezone::tzd());
	ASSERT_TRUE (dt.JulianDayNumber() == 2440587.5);
}

//TODO: write proper test functions

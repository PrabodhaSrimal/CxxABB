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
 * Timespan.h
 *
 * FileId      : $Id: Timespan.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Timespan class
 *
 */

#ifndef CXXABB_CORE_TIMESPAN_H_
#define CXXABB_CORE_TIMESPAN_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Timestamp.h>

namespace CxxAbb {

/** @brief Class for representing span in time (microseconds precision)
 *
 */
class CXXABB_API Timespan
{
public:
	typedef CxxAbb::Timestamp::TimeDiff TimeDiff;

    static const TimeDiff MuSecondsPerMilliSec	= 1000;
    static const TimeDiff MuSecondsPerSec		= 1000 * Timespan::MuSecondsPerMilliSec;
    static const TimeDiff MuSecondsPerMin		=   60 * Timespan::MuSecondsPerSec;
    static const TimeDiff MuSecondsPerHour		=   60 * Timespan::MuSecondsPerMin;
    static const TimeDiff MuSecondsPerDay		=   24 * Timespan::MuSecondsPerHour;

	Timespan()
		: t_Value(0)
	{
	}

	Timespan(TimeDiff _diffInMuSec)
		: t_Value(_diffInMuSec)
	{
	}

	/** @brief To use with timeval struct
	 *
	 */
	Timespan(CxxAbb::UInt32 _seconds, CxxAbb::UInt32 _museconds)
		: t_Value(TimeDiff(_seconds * MuSecondsPerSec) + _museconds)
	{
	}

	Timespan(CxxAbb::Int32 _days, CxxAbb::Int32 _hours, CxxAbb::Int32 _minutes,
		CxxAbb::Int32 _seconds, CxxAbb::Int32 _museconds)
		: t_Value(TimeDiff(_days*MuSecondsPerDay) +
			TimeDiff(_hours*MuSecondsPerHour) +
			TimeDiff(_minutes*MuSecondsPerMin) +
			TimeDiff(_seconds*MuSecondsPerSec) +
			_museconds)
	{
	}

	Timespan(const Timespan& _rhs)
		: t_Value(_rhs.t_Value)
	{
	}

	~Timespan()
	{
	}

	/** @brief Remaining days in timespan [0-...]
	 */
	int Days() const
	{
		return (t_Value/MuSecondsPerDay);
	}

	/** @brief Remaining Hours in timespan [0-23]
	 */
	int Hours() const
	{
		return (t_Value/MuSecondsPerHour) % 24;
	}

	/** @brief Total Hours in timespan
	 */
	int TotalHours() const
	{
		return (t_Value/MuSecondsPerHour);
	}

	/** @brief Remaining minutes in timespan [0-59]
	 */
	int Minutes() const
	{
		return (t_Value/MuSecondsPerMin) % 60;
	}

	/** @brief Total minutes in timespan
	 */
	int TotalMinutes() const
	{
		return (t_Value/MuSecondsPerMin);
	}

	/** @brief Remaining seconds in timespan [0-59]
	 */
	int Seconds() const
	{
		return (t_Value/MuSecondsPerSec) % 60;
	}

	/** @brief Total seconds in timespan
	 */
	int TotalSeconds() const
	{
		return (t_Value/MuSecondsPerSec);
	}

	/** @brief Remaining miliseconds in timespan [0-999]
	 */
	int Milliseconds() const
	{
		return (t_Value/MuSecondsPerMilliSec) % 1000;
	}

	/** @brief Total milliseconds in timespan
	 */
	TimeDiff TotalMilliseconds() const
	{
		return (t_Value/MuSecondsPerMilliSec);
	}

	/** @brief Remaining Microseconds in timespan [0-999]
	 */
	int Microseconds() const
	{
		return (t_Value % 1000);
	}

	/** @brief Total Microseconds in timespan
	 */
	TimeDiff TotalMicroseconds() const
	{
		return t_Value;
	}

	/** @brief Return fraction of seconds in timespan as microseconds
	 */
	int SecondsFraction() const
	{
		return (t_Value % MuSecondsPerSec);
	}

	Timespan& operator = (const Timespan& _timespan)
	{
		t_Value = _timespan.t_Value;
		return *this;
	}

	Timespan& operator = (TimeDiff _diffMuSec)
	{
		t_Value = _diffMuSec;
		return *this;
	}

	Timespan& Set(CxxAbb::Int32 _days, CxxAbb::Int32 _hours, CxxAbb::Int32 _minutes,
		CxxAbb::Int32 _seconds, CxxAbb::Int32 _museconds)
	{
		t_Value = TimeDiff(_days*MuSecondsPerDay) +
					TimeDiff(_hours*MuSecondsPerHour) +
					TimeDiff(_minutes*MuSecondsPerMin) +
					TimeDiff(_seconds*MuSecondsPerSec) +
					_museconds;
		return *this;
	}

	Timespan& Set(CxxAbb::UInt32 _seconds, CxxAbb::UInt32 _museconds)
	{
		t_Value = TimeDiff(_seconds * MuSecondsPerSec) + _museconds;
		return *this;
	}

	void Swap(Timespan& _timespan)
	{
		std::swap(t_Value, _timespan.t_Value);
	}

	bool operator == (const Timespan& _span) const
	{
		return (t_Value == _span.t_Value);
	}

	bool operator != (const Timespan& _span) const
	{
		return (t_Value != _span.t_Value);
	}

	bool operator >  (const Timespan& _span) const
	{
		return (t_Value > _span.t_Value);
	}

	bool operator >= (const Timespan& _span) const
	{
		return (t_Value >= _span.t_Value);
	}

	bool operator <  (const Timespan& _span) const
	{
		return (t_Value < _span.t_Value);
	}

	bool operator <= (const Timespan& _span) const
	{
		return (t_Value <= _span.t_Value);
	}

	bool operator == (TimeDiff _museconds) const
	{
		return (t_Value == _museconds);
	}

	bool operator != (TimeDiff _museconds) const
	{
		return (t_Value != _museconds);
	}

	bool operator >  (TimeDiff _museconds) const
	{
		return (t_Value > _museconds);
	}

	bool operator >= (TimeDiff _museconds) const
	{
		return (t_Value >= _museconds);
	}

	bool operator <  (TimeDiff _museconds) const
	{
		return (t_Value < _museconds);
	}

	bool operator <= (TimeDiff _museconds) const
	{
		return (t_Value <= _museconds);
	}

	Timespan operator + (const Timespan& _span) const
	{
		return Timespan(t_Value + _span.t_Value);
	}

	Timespan operator - (const Timespan& _span) const
	{
		return Timespan(t_Value - _span.t_Value);
	}

	Timespan& operator += (const Timespan& _span)
	{
		t_Value += _span.t_Value;
		return *this;
	}

	Timespan& operator -= (const Timespan& _span)
	{
		t_Value -= _span.t_Value;
		return *this;
	}

	Timespan operator + (TimeDiff _museconds) const
	{
		return Timespan(t_Value + _museconds);
	}

	Timespan operator - (TimeDiff _museconds) const
	{
		return Timespan(t_Value - _museconds);
	}

	Timespan& operator += (TimeDiff _museconds)
	{
		t_Value += _museconds;
		return *this;
	}

	Timespan& operator -= (TimeDiff _museconds)
	{
		t_Value += _museconds;
		return *this;
	}

private:
	TimeDiff t_Value;
};

}  // namespace CxxAbb


#endif /* CXXABB_CORE_TIMESPAN_H_ */

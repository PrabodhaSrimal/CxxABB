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
 * Timestamp.h
 *
 * FileId      : $Id: Timestamp.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 16, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Timestamp representation
 *
 */

#ifndef CXXABB_CORE_TIMESTAMP_H_
#define CXXABB_CORE_TIMESTAMP_H_

#include <CxxAbb/Core.h>
#include <ctime>

namespace CxxAbb
{


/** @brief Timestamp class for representing instance in time (Microsecond Resolution)
 *  This class also support difference between two instance in time
 *  The void Now() method updates the timestamp for current instance of time.
 *  Base point of timestamp may vary in platform implementation. Usually its Epoch.
 */
class CXXABB_API Timestamp
{
public:
	typedef CxxAbb::Int64 TimeVal;    /// monotonic time value in microsecond resolution (Epoch)
	/// 100 nanosecond units since  1582-10-15 Midnight (Gregorian Calendar Start)
	typedef CxxAbb::Int64 UtcTimeVal;
	typedef CxxAbb::Int64 TimeDiff;   /// difference between two Timestamps in microseconds

	static const CxxAbb::UInt32 MuSecPerSecond = 1000000;

	/** @brief Creates timestamp for NOW
	 *
	 */
	Timestamp();

	/** @brief Create timestamp from timeval
	 */
	Timestamp(TimeVal _timeval);

	/** @brief Copy ctor
	 *
	 */
	Timestamp(const Timestamp& _other);

	/** @brief Dtor
	 *
	 */
	~Timestamp();

	/** @brief Set current timestamp for NOW
	 * This has platform specific impl
	 */
	void Now();

	/** @brief Swap
	 *
	 */
	void Swap(Timestamp& _timestamp);

	/** @brief Operators
	 *
	 */
	Timestamp& operator = (const Timestamp& _other);
	Timestamp& operator = (TimeVal _tv);
	bool operator == (const Timestamp& _ts) const;
	bool operator != (const Timestamp& _ts) const;
	bool operator >  (const Timestamp& _ts) const;
	bool operator >= (const Timestamp& _ts) const;
	bool operator <  (const Timestamp& _ts) const;
	bool operator <= (const Timestamp& _ts) const;
	Timestamp  operator +  (TimeDiff _diff) const;
	Timestamp  operator -  (TimeDiff _diff) const;
	TimeDiff   operator -  (const Timestamp& _ts) const;
	Timestamp& operator += (TimeDiff _diff);
	Timestamp& operator -= (TimeDiff _diff);

	/** @brief Get Epoch representation of current timestamp
	 * @return std::time_t Epoch time in time_t (seconds)
	 * Epoch is based on 1970-01-01 Midnight
	 */
	std::time_t Epoch() const;

	/** @brief Get UTC representation of current timestamp
	 * @return UtcTimeVal 100 nanoseconds units since UTC base
	 */
	UtcTimeVal Utc() const;

	/** @brief Get Epoch representation of current timestamp in Microseconds
	 * @return TimeVal Epoch time in Microseconds
	 * Epoch is based on 1970-01-01 Midnight
	 */
	TimeVal EpochMicroseconds() const;

	/** @brief Get elapsed time in microseconds
	 * @return TimeDiff Now - *this
	 */
	TimeDiff Elapsed() const;

	/** @brief Check if given interval is elapsed
	 *
	 */
	bool IsElapsed(TimeDiff _interval) const;

	/** @brief Converters from Epoch
	 *
	 */
	static Timestamp FromEpoch(std::time_t t);

	/** @brief Converters from UTC
	 *
	 */
	static Timestamp FromUtc(UtcTimeVal val);

	/** @brief Resolution of the timestamp
	 *  Always 1000000
	 */
	static TimeVal Resolution()
	{
		return MuSecPerSecond;
	}

private:

	/// Current timestamp in microsecond resolution. Base point is not considered.
	TimeVal t_Value;
};

// inlines
inline void Timestamp::Swap(Timestamp& _timestamp)
{
	std::swap(t_Value, _timestamp.t_Value);
}

inline Timestamp& Timestamp::operator = (const Timestamp& _other)
{
	t_Value = _other.t_Value;
	return *this;
}

inline Timestamp& Timestamp::operator = (TimeVal _tv)
{
	t_Value = _tv;
	return *this;
}

inline bool Timestamp::operator == (const Timestamp& _ts) const
{
	return (t_Value == _ts.t_Value);
}

inline bool Timestamp::operator !=(const Timestamp& _ts) const
{
	return (t_Value != _ts.t_Value);
}

inline bool Timestamp::operator >  (const Timestamp& _ts) const
{
	return (t_Value > _ts.t_Value);
}

inline bool Timestamp::operator >= (const Timestamp& _ts) const
{
	return (t_Value >= _ts.t_Value);
}

inline bool Timestamp::operator <  (const Timestamp& _ts) const
{
	return (t_Value < _ts.t_Value);
}

inline bool Timestamp::operator <= (const Timestamp& _ts) const
{
	return (t_Value <= _ts.t_Value);
}

inline Timestamp  Timestamp::operator +  (TimeDiff _diff) const
{
	return (t_Value + _diff);
}

inline Timestamp  Timestamp::operator -  (TimeDiff _diff) const
{
	return (t_Value - _diff);
}

inline Timestamp::TimeDiff Timestamp::operator -  (const Timestamp& _ts) const
{
	return TimeDiff(t_Value - _ts.t_Value);
}

inline Timestamp& Timestamp::operator += (TimeDiff _diff)
{
	t_Value += _diff;
	return *this;
}

inline Timestamp& Timestamp::operator -= (TimeDiff _diff)
{
	t_Value -= _diff;
	return *this;
}


} /* namespace CxxAbb */


#endif /* CXXABB_CORE_TIMESTAMP_H_ */

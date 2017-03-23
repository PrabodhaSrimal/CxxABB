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
 * TimeZone.h
 *
 * FileId      : $Id: TimeZone.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 25, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : TimeZone check functionality
 *
 */

#ifndef CXXABB_CORE_TIMEZONE_H_
#define CXXABB_CORE_TIMEZONE_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/DateTime.h>
#include <CxxAbb/Timestamp.h>

namespace CxxAbb
{

class DateTime;
class LocalDateTime;

/** @brief TimeZone Info Provider for local timezone
 * LocatTime = UTC + UtcOffset() + DstCorrection();
 */
class CXXABB_API TimeZone
{
public:
	/** @brief Get current timezone offset in seconds from UTC
	 */
	static int UtcOffset();

	/** @brief Get Day Light Savings for current timezone in seconds
	 */
	static int DstCorrection();

	/** @brief Check if current timezone is in effect of Day Light Saving
	 */
	static bool IsDst(const CxxAbb::Timestamp& _timestamp);

	/** @brief Get total time diff for current timezone from UTC in seconds
	 *  UtcOffset() + DstCorrection();
	 */
	static inline int TimeZoneDiff()
	{
		return UtcOffset() + DstCorrection();
	}

	/** @brief Get correct timezone name for current timezone
	 */
	static std::string TimeZoneName();

	/** @brief Get standard (non-dst) timezone name for current timezone
	 */
	static std::string TimeZoneNameStandard();

	/** @brief Get DST timezone name for current timezone
	 */
	static std::string TimeZoneNameDst();
};

/** @brief TimeZone Info Provider for given timezone
 * Refer following to timezone name format
 * http://pubs.opengroup.org/onlinepubs/007904875/basedefs/xbd_chap08.html
 * http://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
 * In most *NIX systems zoneinfo files are in /usr/share/zoneinfo/ folder
 */
class TimeZoneInfo
{
public:
	/** @brief Timezone info for given timezone
	 */
	TimeZoneInfo(const char * _zZoneName = ":GMT");

	TimeZoneInfo(const TimeZoneInfo & _rhs);

	/** @brief Get timezone difference = utcoffset + dst
	 */
	int GetTimeZoneDiff(time_t _epoch);

	/** @brief Check if given time in DST
	 */
	bool IsDST(time_t _epoch);

	inline const char * GetZoneName() const { return s_Zone.c_str(); }

	inline void Swap(TimeZoneInfo & _rhs)
	{
		std::swap(s_Zone, _rhs.s_Zone);
	}

	inline TimeZoneInfo& operator = (const TimeZoneInfo& _rhs)
	{
		if(&_rhs != this)
		{
			s_Zone = _rhs.s_Zone;
		}
		return *this;
	}

private:
	void ResetTz();
	void SetTz();
	void CalculateTzd();

	std::string s_Zone;
};

}  /* namespace CxxAbb */

#endif /* CXXABB_CORE_TIMEZONE_H_ */

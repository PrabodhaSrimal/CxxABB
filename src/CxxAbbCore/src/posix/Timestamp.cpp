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
 * Comment     : Timestamp representation for POSIX specific platforms
 *
 */

#include <CxxAbb/Timestamp.h>
#include <CxxAbb/Exception.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <errno.h>

namespace CxxAbb
{

void Timestamp::Now()
{
	struct timeval tv;
	if (::gettimeofday(&tv, NULL))
	{
		std::stringstream ss;
		ss << "gettimeofday() failed : ";
		ss << strerror(errno);
		throw CxxAbb::SystemException(ss.str());
	}

	t_Value = TimeVal(tv.tv_sec) * MuSecPerSecond + tv.tv_usec;
}

} /* namespace CxxAbb */




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
 * Comment     : TimeZone check functionality
 *
 */

#include <CxxAbb/TimeZone.h>

namespace CxxAbb
{


}  /* namespace CxxAbb */

#if (CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_UNIX) || (CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_BSD)

#include "posix/TimeZone.cpp"

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_WINDOWS

#include "win32/TimeZone.cpp"

#endif


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
 * Platform.cpp
 *
 * FileId      : $Id: Platform.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Global platform identifications and definitions
 *
 */

#include <CxxAbb/Platform.h>

namespace CxxAbb
{

const char * PlatformOsName(int _iOsId)
{
	switch(_iOsId)
	{
		case CXXABB_OS_LINUX:
			return "Linux";
		case CXXABB_OS_WINDOWS:
			return "Windows";
		case CXXABB_OS_CYGWIN:
			return "Cygwin";
		case CXXABB_OS_BSD:
			return "BSD";
		case CXXABB_OS_SOLARIS:
			return "Solaris";
		default:
			return "Unknown";
	}
}

const char * PlatformOsFamilyName(int _iOsFamilyid)
{
	switch(_iOsFamilyid)
	{
		case CXXABB_OS_FAMILY_UNIX:
			return "Unix";
		case CXXABB_OS_FAMILY_WINDOWS:
			return "Windows";
		case CXXABB_OS_FAMILY_BSD:
			return "BSD";
		default:
			return "Unknown";
	}
}

const char * PlatformArchName(int _iArchId)
{
	switch(_iArchId)
	{
		case CXXABB_ARCH_IA32:
			return "IA32";
		case CXXABB_ARCH_IA64:
			return "IA64";
		case CXXABB_ARCH_AMD64:
			return "AMD64";
		default:
			return "Unknown";
	}
}

bool IsPlatformLittleEndian()
{
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	return true;
#else
	return false;
#endif
}

} /* namespace CxxAbb */




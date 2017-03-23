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
 * Debug.cpp
 *
 * FileId      : $Id: Debug.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 14, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Debugging facilities - Macros and helper functions
 *
 */

#include <CxxAbb/Debug.h>
#include <CxxAbb/Exception.h>
#include <sstream>
#include <iostream>

namespace CxxAbb
{

void Debugger::Assert(const char* _zCond, const char* _zFile, int _iLine)
{
	throw AssertionViolationException(what(_zCond, _zFile, _iLine));
}

void Debugger::NullPointer(const char* _zPtr, const char* _zFile, int _iLine)
{
	throw NullPointerException(what(_zPtr, _zFile, _iLine));
}

void Debugger::Debug(const char* _zMsg, const char* _zFile, int _iLine)
{
	std::fputs(what(_zMsg, _zFile, _iLine).c_str(),stderr);
}

void Debugger::DebugBreak(const char* _zMsg, const char* _zFile, int _iLine)
{
	throw DebuggerException(what(_zMsg, _zFile, _iLine));
}

std::string Debugger::what(const char* _zMsg, const char* _zFile, int _iLine)
{
	std::stringstream oss;
	oss << "<DEBUG> [" << _zFile << ":" << _iLine << "] ";
	if (_zMsg)
	{
		oss << _zMsg;
	}
	oss << std::endl;
	return oss.str();
}

}


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
 * ExceptionHandler.cpp
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 21, 2012
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Global Exception Handlers
 *
 */

#include <CxxAbb/ExceptionHandler.h>
#include <CxxAbb/Debug.h>
#include <CxxAbb/Sys/Mutex.h>
#include <CxxAbb/Sys/ScopedLock.h>

namespace CxxAbb
{

ExceptionHandler* ThreadErrorHandler::p_CurrentHandler = new CxxAbb::ExceptionHandler;
CxxAbb::Sys::FastMutex ThreadErrorHandler::m_Mutex;

void ExceptionHandler::Exception(const CxxAbb::Exception & _e)
{
	DEBUGLOG(_e.what());
}

void ExceptionHandler::Exception(const std::exception & _e)
{
	DEBUGLOG(_e.what());
}

void ExceptionHandler::Exception()
{
	DEBUGLOG("Unhandled Exception");
}

ExceptionHandler * ThreadErrorHandler::Set(ExceptionHandler * _new)
{
	CHECKNULL(_new);

	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);
	ExceptionHandler * old = p_CurrentHandler;
	p_CurrentHandler = _new;
	return old;
}

ExceptionHandler * ThreadErrorHandler::Get()
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);
	return p_CurrentHandler;
}

void ThreadErrorHandler::Handle(const CxxAbb::Exception & _e)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	try
	{
		p_CurrentHandler->Exception(_e);
	}
	catch(...)
	{
	}
}

void ThreadErrorHandler::Handle(const std::exception & _e)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	try
	{
		p_CurrentHandler->Exception(_e);
	}
	catch(...)
	{
	}
}

void ThreadErrorHandler::Handle()
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	try
	{
		p_CurrentHandler->Exception();
	}
	catch(...)
	{
	}
}

}  /* namespace CxxAbb */


 

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
 * Thread.cpp
 *
 * FileId      : $Id: Thread.cpp 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 19, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Platform independent thread wrapper
 *
 */

#include <CxxAbb/Sys/Thread.h>
#include <CxxAbb/Sys/Mutex.h>
#include <CxxAbb/Sys/ScopedLock.h>
#include <sstream>

namespace CxxAbb
{

namespace Sys
{

Thread::Thread() : s_Name(GenName())
{
	SetName();
}

Thread::Thread(std::string _name) : s_Name(_name)
{
	SetName();
}

Thread::~Thread()
{
}

CxxAbb::UInt32 Thread::Tid() const
{
	return (CxxAbb::UInt32)TidImpl();
}

std::string & Thread::Name()
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(mtx_LocalData);
	return s_Name;
}

void Thread::Name(std::string _name)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(mtx_LocalData);
	s_Name = _name;
	SetName();
}

std::string Thread::GenName()
{
	std::stringstream ss;
	ss << "Thr";
	ss << Tid();

	return ss.str();
}

void Thread::SetName()
{
	SetNameImpl(s_Name);
}

Thread::State Thread::CurrentState()
{
	return State(CurrentStateImpl());
}

void Thread::Start(CxxAbb::Runnable & _runnable)
{
	StartImpl(_runnable);
}

void Thread::Start(Callable _callable, void * _data)
{
	StartImpl(_callable, _data);
}

void * Thread::Join()
{
	return JoinImpl();
}

bool Thread::TryJoin(long _lMilliSeconds, void * _status)
{
	return TryJoinImpl(_lMilliSeconds, _status);
}

void Thread::Detach()
{
	DetachImpl();
}

void Thread::Yield()
{
	YieldImpl();
}

void Thread::Sleep(long _lMilliSeconds)
{
	SleepImpl(_lMilliSeconds);
}

Thread * Thread::Current()
{
	return static_cast<Thread*>(CurrentImpl());
}

}  /* namespace Sys */

}  /* namespace CxxAbb */

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
 * Thread.h
 *
 * FileId      : $Id: Thread.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 18, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Platform independent thread wrapper
 *
 */

#ifndef CXXABB_CORE_THREAD_H_
#define CXXABB_CORE_THREAD_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include <CxxAbb/Runnable.h>
#include <CxxAbb/Sys/Mutex.h>
#include <CxxAbb/Sys/ScopedLock.h>
#include "ThreadImpl.h"

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API Thread : public NonCopyable, private ThreadImpl
{
public:
	enum Priority
	{
		Lowest = Lowest_impl,
		Low = Low_impl,
		Normal = Normal_impl,
		High = High_impl,
		Highest = Highest_impl
	};

	enum State
	{
		Ready = Ready_impl,
		Running = Running_impl,
		Finish = Finish_impl
	};

	typedef ThreadImpl::Callable Callable;

	/** @brief Create thread without name
	 *  name will be auto genereated
	 */
	Thread();

	/** @brief Create thread with a name
	 *  name will be shown in monitoring tools and used in logging
	 */
	Thread(std::string _name);

	virtual ~Thread();

	/** @brief Get thread unique ID
	 */
	CxxAbb::UInt32 Tid() const;

	std::string & Name();

	void Name(std::string _name);

	Thread::State CurrentState();

	bool IsRunning() { return CurrentState() == Running; }

	bool IsFinish() { return CurrentState() == Finish; }

	void Start(CxxAbb::Runnable & _runnable);

	void Start(Callable _callable, void * _data = 0);

	void * Join();

	bool TryJoin(long _lMilliSeconds, void * _status = NULL);

	void Detach();

	static void Yield();

	static void Sleep(long _lMilliSeconds);

	/// Static functions

	static Thread * Current();

private:
	std::string GenName();
	void SetName();

	std::string s_Name;
	CxxAbb::Sys::FastMutex mtx_LocalData;
};

}  /* namespace Sys */

}  /* namespace CxxAbb */

#endif /* CXXABB_CORE_THREAD_H_ */

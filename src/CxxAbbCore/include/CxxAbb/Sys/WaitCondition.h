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
 * WaitCondition.h
 *
 * FileId      : $Id: WaitCondition.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Condition which thread can wait and/or signal
 *
 */

#ifndef CXXABB_CORE_WAITCONDITION_H_
#define CXXABB_CORE_WAITCONDITION_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Sys/Mutex.h>
#include <CxxAbb/Sys/ScopedLock.h>
#include <CxxAbb/Sys/ScopedUnlock.h>
#include <CxxAbb/Sys/SigEvent.h>
#include <list>

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API WaitCondition
{
public:
	WaitCondition()
	{}

	~WaitCondition()
	{}

	/** @brief Wait on this condition to be signaled
	 * First to call will be first to signaled (FIFO)
	 * First to call semantic can be enforced by locking an external lock (_mtxCall)
	 * and call this method
	 */
	template <class Mutex>
	void Wait(Mutex & _mtxCall)
	{
		CxxAbb::Sys::ScopedUnlock<Mutex> lckCall(_mtxCall, false);
		CxxAbb::Sys::SigEvent evt;
		{
			CxxAbb::Sys::FastMutex::ScopedLock lckQueue(mtx_Queue);
			lckCall.Unlock();
			m_WaitQueue.push_back(&evt);
		}
		evt.Wait();
	}

	template <class Mutex>
	void TryWait(Mutex & _mtxCall, long _lMilliseconds)
	{
		CxxAbb::Sys::ScopedUnlock<Mutex> lckCall(_mtxCall, false);
		CxxAbb::Sys::SigEvent evt;
		{
			CxxAbb::Sys::FastMutex::ScopedLock lckQueue(mtx_Queue);
			lckCall.Unlock();
			m_WaitQueue.push_back(&evt);
		}

		if(!evt.TryWait(_lMilliseconds))
		{
			CxxAbb::Sys::FastMutex::ScopedLock lckQueue(mtx_Queue);
			RemoveEvent(evt);
			return false;
		}

		return true;
	}

	/** @brief Signal next in queue event
	 */
	void Signal();

	/** @brief Signal all waiting events
	 */
	void SignalAll();

private:
	void RemoveEvent(CxxAbb::Sys::SigEvent & _evt);

	typedef std::list<CxxAbb::Sys::SigEvent*> EventQueue;

	CxxAbb::Sys::FastMutex mtx_Queue;
	EventQueue m_WaitQueue;
};

}  /* namespace Sys */

}  /* namespace CxxAbb */

#endif /* CXXABB_CORE_WAITCONDITION_H_ */

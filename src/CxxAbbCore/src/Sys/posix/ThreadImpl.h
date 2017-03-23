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
 * ThreadImpl.h
 *
 * FileId      : $Id: ThreadImpl.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 18, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Platform independent thread wrapper - POSIX ipmlimentation
 *
 */

#ifndef CXXABB_CORE_THREADIMPL_H_
#define CXXABB_CORE_THREADIMPL_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Sys/SigEvent.h>
#include <CxxAbb/SmartPtr.h>
#include <CxxAbb/RefCountedObj.h>
#include <CxxAbb/Runnable.h>
#include <CxxAbb/Sys/SignalToException.h>
#include <pthread.h>

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API ThreadImpl
{
public:
	typedef pthread_t ThreadId;

	typedef void (*Callable) (void *);

	enum Priority
	{
		Lowest_impl = 0,
		Low_impl,
		Normal_impl,
		High_impl,
		Highest_impl
	};

	enum State
	{
		Ready_impl = 0,
		Running_impl,
		Finish_impl
	};

	class CallableData : public CxxAbb::RefCounted
	{
	public:
		CallableData() :
			fp_Callback(NullPtr),
			p_Data(NullPtr)
		{}

		ThreadImpl::Callable fp_Callback;
		void * p_Data;
	};

protected:
	ThreadImpl();

	virtual ~ThreadImpl();

	ThreadId TidImpl() const
	{
		return ptr_ThreadData->t_ThreadHandle;
	}

	void SetNameImpl(std::string & _sName);

	State CurrentStateImpl();

	void StartImpl(CxxAbb::Runnable & _runnable);

	void StartImpl(Callable _callable, void * _data);

	void * JoinImpl();

	bool TryJoinImpl(long _lMilliSeconds, void * _status);

	void DetachImpl();

	static void YieldImpl();

	static void SleepImpl(long _lMilliSeconds);

	static ThreadImpl * CurrentImpl();

	static void * ThreadRunnableEntry(void * _thread);

	static void * ThreadCallableEntry(void * _thread);

private:

	/** @brief Helper class to store thread specific data in thread context
	 */
	class ThreadContext
	{
	public:
		ThreadContext()
		{
			int i = pthread_key_create(&t_Key, NULL);
			if(i)
			{
				throw CxxAbb::SystemException("pthread_key_create failed", i);
			}
		}

		~ThreadContext()
		{
			pthread_key_delete(t_Key);
		}

		ThreadImpl * Get() const
		{
			return reinterpret_cast<ThreadImpl*>(pthread_getspecific(t_Key));
		}

		void Set(ThreadImpl * _pThread)
		{
			pthread_setspecific(t_Key, _pThread);
		}
	private:
		pthread_key_t t_Key;
	};

	/** @brief Thread Specific data. To be used with Smart Ponters
	 */
	class ThreadData : public CxxAbb::RefCounted
	{
	public:
		ThreadData()
			: p_Runnable(NullPtr),
			  ptr_CallableData(NullPtr),
			  t_ThreadHandle(0),
			  e_Priority(Normal_impl),
			  t_StackSize(0),
			  m_Event(false),
			  e_State(ThreadImpl::Ready_impl)
		{
		}

		CxxAbb::Runnable * p_Runnable;
		CxxAbb::AutoPtr<ThreadImpl::CallableData> ptr_CallableData;
		pthread_t t_ThreadHandle;
		Priority e_Priority;
		std::size_t t_StackSize;
		CxxAbb::Sys::SigEvent m_Event;
		ThreadImpl::State e_State;

	private:
	};

	int PriorityMapper(Priority _pri);

	AutoPtr<ThreadData> ptr_ThreadData;

	static ThreadContext m_ThreadHolder;

#if defined(CXXABB_OS_FAMILY_UNIX)
	CxxAbb::Sys::SignalToException::JumpBufferVec m_JumpBuffers;
	friend class CxxAbb::Sys::SignalToException;
#endif
};

}  /* namespace Sys */

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_THREADIMPL_H_ */

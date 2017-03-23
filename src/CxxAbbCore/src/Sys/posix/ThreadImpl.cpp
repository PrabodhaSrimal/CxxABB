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
 * ThreadImpl.cpp
 *
 * FileId      : $Id: ThreadImpl.cpp 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 19, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Platform independent thread wrapper - POSIX ipmlimentation
 *
 */

#include "ThreadImpl.h"
#include <CxxAbb/Timespan.h>
#include <CxxAbb/Timestamp.h>
#include <CxxAbb/ExceptionHandler.h>
#include <signal.h> // for signal handling
#include <errno.h> // for errno
#include <sys/prctl.h> // for name set

namespace CxxAbb
{

namespace Sys
{

ThreadImpl::ThreadContext ThreadImpl::m_ThreadHolder;

ThreadImpl::ThreadImpl()
	: ptr_ThreadData(new ThreadData)
{
}

ThreadImpl::~ThreadImpl()
{
	if(CurrentStateImpl() == Running_impl)
	{
		pthread_detach(ptr_ThreadData->t_ThreadHandle);
	}
}

void ThreadImpl::SetNameImpl(std::string & _sName)
{
	char zTname[16];
	strncpy(zTname, _sName.c_str(), 16);
	prctl(PR_SET_NAME, zTname);
}

ThreadImpl::State ThreadImpl::CurrentStateImpl()
{
	return ptr_ThreadData->e_State;
}

void ThreadImpl::StartImpl(CxxAbb::Runnable & _runnable)
{
	if(ptr_ThreadData->e_State == Running_impl)
	{
		throw CxxAbb::SystemException("Thread is already running");
	}

	pthread_attr_t tAttr;
	pthread_attr_init(&tAttr);

	if (ptr_ThreadData->t_StackSize != 0)
	{
		int rc = pthread_attr_setstacksize(&tAttr, ptr_ThreadData->t_StackSize);
		if (0 != rc)
		{
			pthread_attr_destroy(&tAttr);
			throw CxxAbb::SystemException("pthread_attr_setstacksize failed", rc);
		}
	}

	ptr_ThreadData->p_Runnable = &_runnable;
	int rc = pthread_create(&ptr_ThreadData->t_ThreadHandle, &tAttr, ThreadRunnableEntry, this);
	if (rc)
	{
		ptr_ThreadData->p_Runnable = NullPtr;
		pthread_attr_destroy(&tAttr);
		throw CxxAbb::SystemException("pthread_create failed", rc);
	}
	pthread_attr_destroy(&tAttr);

	if (ptr_ThreadData->e_Priority != Normal_impl)
	{
		struct sched_param spar;
		spar.sched_priority = PriorityMapper(ptr_ThreadData->e_Priority);
		rc = pthread_setschedparam(ptr_ThreadData->t_ThreadHandle, SCHED_OTHER, &spar);
		if (rc)
		{
			throw CxxAbb::SystemException("pthread_setschedparam failed", rc);
		}
	}


}

void ThreadImpl::StartImpl(Callable _callable, void * _data)
{
	if(ptr_ThreadData->e_State == Running_impl)
	{
		throw CxxAbb::SystemException("Thread is already running");
	}

	pthread_attr_t tAttr;
	pthread_attr_init(&tAttr);

	if (ptr_ThreadData->t_StackSize != 0)
	{
		int rc = pthread_attr_setstacksize(&tAttr, ptr_ThreadData->t_StackSize);
		if (0 != rc)
		{
			pthread_attr_destroy(&tAttr);
			throw CxxAbb::SystemException("pthread_attr_setstacksize failed", rc);
		}
	}

	if(ptr_ThreadData->ptr_CallableData.get() == NullPtr)
		ptr_ThreadData->ptr_CallableData = new CallableData;

	ptr_ThreadData->ptr_CallableData->fp_Callback = _callable;
	ptr_ThreadData->ptr_CallableData->p_Data = _data;

	int rc = pthread_create(&ptr_ThreadData->t_ThreadHandle, &tAttr, ThreadCallableEntry, this);
	if (rc)
	{
		ptr_ThreadData->ptr_CallableData->fp_Callback = NullPtr;
		ptr_ThreadData->ptr_CallableData->p_Data = NullPtr;
		pthread_attr_destroy(&tAttr);
		throw CxxAbb::SystemException("pthread_create failed", rc);
	}
	pthread_attr_destroy(&tAttr);

	if (ptr_ThreadData->e_Priority != Normal_impl)
	{
		struct sched_param spar;
		spar.sched_priority = PriorityMapper(ptr_ThreadData->e_Priority);
		rc = pthread_setschedparam(ptr_ThreadData->t_ThreadHandle, SCHED_OTHER, &spar);
		if (rc)
		{
			throw CxxAbb::SystemException("pthread_setschedparam failed", rc);
		}
	}
}

void * ThreadImpl::JoinImpl()
{
	ptr_ThreadData->m_Event.Wait(); // wait for the thread func complete

	void * pExitStatus;
	int i = pthread_join(ptr_ThreadData->t_ThreadHandle, &pExitStatus);
	if(i)
	{
		throw CxxAbb::SystemException("pthread_join failed", i);
	}
	return pExitStatus;
}

bool ThreadImpl::TryJoinImpl(long _lMilliSeconds, void * _status)
{
	if(ptr_ThreadData->m_Event.TryWait(_lMilliSeconds))
	{
		int i = pthread_join(ptr_ThreadData->t_ThreadHandle, &_status);
		if(i)
		{
			throw CxxAbb::SystemException("pthread_join failed", i);
		}
		return true;
	}
	else
	{
		_status = NullPtr;
		return false;
	}
}

void ThreadImpl::DetachImpl()
{
	int i = pthread_detach(ptr_ThreadData->t_ThreadHandle);
	if(i)
	{
		throw CxxAbb::SystemException("pthread_detach failed", i);
	}
}

void ThreadImpl::YieldImpl()
{
	pthread_yield(); // calls sched_yield() so no errors :)
}

void ThreadImpl::SleepImpl(long _lMilliSeconds)
{
	CxxAbb::Timespan tRemain(1000*CxxAbb::Timespan::TimeDiff(_lMilliSeconds));
	int rc;
	do
	{
		struct timespec ts;
		ts.tv_sec  = (long) tRemain.TotalSeconds();
		ts.tv_nsec = (long) tRemain.SecondsFraction() * 1000;

		CxxAbb::Timestamp start;

		rc = ::nanosleep(&ts, 0);
		if (rc < 0 && errno == EINTR)
		{
			CxxAbb::Timestamp end;
			CxxAbb::Timespan elapsed = start.Elapsed();
			if (elapsed < tRemain)
				tRemain -= elapsed;
			else
				tRemain = 0;
		}
	}
	while (tRemain > 0 && rc < 0 && errno == EINTR);

	if (rc < 0 && tRemain > 0)
	{
		throw CxxAbb::SystemException("Thread::SleepImpl nanosleep failed", errno);
	}
}

ThreadImpl * ThreadImpl::CurrentImpl()
{
	return m_ThreadHolder.Get();
}

void * ThreadImpl::ThreadRunnableEntry(void * _thread)
{
	ThreadImpl* pImpl = reinterpret_cast<ThreadImpl*>(_thread);

	m_ThreadHolder.Set(pImpl);

	CxxAbb::AutoPtr<ThreadData> pData = pImpl->ptr_ThreadData;

	pData->e_State = Running_impl;

	try
	{
		pData->p_Runnable->Run();
	}
	catch(CxxAbb::Exception & ex)
	{
		CxxAbb::ThreadErrorHandler::Handle(ex);
	}
	catch(std::exception & ex)
	{
		CxxAbb::ThreadErrorHandler::Handle(ex);
	}
	catch(...)
	{
		CxxAbb::ThreadErrorHandler::Handle();
	}

	pData->p_Runnable = NullPtr;
	pData->e_State = Finish_impl;
	pData->m_Event.Set();
	return NULL;
}

void * ThreadImpl::ThreadCallableEntry(void * _thread)
{
	ThreadImpl* pImpl = reinterpret_cast<ThreadImpl*>(_thread);

	m_ThreadHolder.Set(pImpl);

	CxxAbb::AutoPtr<ThreadData> pData = pImpl->ptr_ThreadData;

	pData->e_State = Running_impl;

	try
	{
		pData->ptr_CallableData->fp_Callback(pData->ptr_CallableData->p_Data);
	}
	catch(CxxAbb::Exception & ex)
	{
		CxxAbb::ThreadErrorHandler::Handle(ex);
	}
	catch(std::exception & ex)
	{
		CxxAbb::ThreadErrorHandler::Handle(ex);
	}
	catch(...)
	{
		CxxAbb::ThreadErrorHandler::Handle();
	}

	pData->ptr_CallableData->fp_Callback = NullPtr;
	pData->ptr_CallableData->p_Data = NullPtr;
	pData->e_State = Finish_impl;
	pData->m_Event.Set();
	return NULL;
}

int ThreadImpl::PriorityMapper(Priority _pri)
{
	int iMin = sched_get_priority_min(SCHED_OTHER);
	int iMax = sched_get_priority_max(SCHED_OTHER);

	switch (_pri)
	{
		case Lowest_impl:
			return iMin;
		case Low_impl:
			return iMin + (iMax - iMin) / 4;
		case Normal_impl:
			return iMin + (iMax - iMin) / 2;
		case High_impl:
			return iMin + 3 * (iMax - iMin) / 4;
		case Highest_impl:
			return iMax;
		default:
			break;
	}

	return -1;
}

}  /* namespace Sys */

}  /* namespace CxxAbb */

 

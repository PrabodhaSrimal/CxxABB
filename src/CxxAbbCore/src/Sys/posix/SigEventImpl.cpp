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
 * SigEventImpl.cpp
 *
 * FileId      : $Id: SigEventImpl.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 12, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Signaling Event for synchronization waiting on a WaitCondition - POSIX impl
 *
 */

#include "SigEventImpl.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h> // posix defines
#include <sys/time.h> // *nix specific


namespace CxxAbb
{

namespace Sys
{

SigEventImpl::SigEventImpl(bool _autoreset)
	: b_AutoReset(_autoreset), b_EventHappened(false)
{
	if (pthread_mutex_init(&mtx_Lock, NULL))
		throw SystemException("pthread_mutex_init failed");
	if (pthread_cond_init(&cnd_Cond, NULL))
		throw SystemException("pthread_cond_init failed");
}

SigEventImpl::~SigEventImpl()
{
	pthread_cond_destroy(&cnd_Cond);
	pthread_mutex_destroy(&mtx_Lock);
}

void SigEventImpl::WaitImpl()
{
	// get lock
	if (pthread_mutex_lock(&mtx_Lock))
		throw SystemException("wait for event failed (lock)");

	// wait until event occur
	while (!b_EventHappened)
	{
		if (pthread_cond_wait(&cnd_Cond, &mtx_Lock))
		{
			pthread_mutex_unlock(&mtx_Lock);
			throw SystemException("wait for event failed");
		}
	}

	// auto reset event
	if (b_AutoReset)
		b_EventHappened = false;

	// release lock
	pthread_mutex_unlock(&mtx_Lock);
}

bool SigEventImpl::TryWaitImpl(long _lMiliSeconds)
{
	struct timespec abstime;
	struct timeval tv;
	int rc;

	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + _lMiliSeconds / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (_lMiliSeconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	if (pthread_mutex_lock(&mtx_Lock) != 0)
		throw SystemException("pthread_mutex_lock failed");
	while (!b_EventHappened)
	{
		if ((rc = pthread_cond_timedwait(&cnd_Cond, &mtx_Lock, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&mtx_Lock);
			throw SystemException("pthread_cond_timedwait ETIMEDOUT");
		}
	}
	if (rc == 0 && b_AutoReset) b_EventHappened = false;
	pthread_mutex_unlock(&mtx_Lock);
	return rc == 0;
}

}  /* namespace Sys */

}  /* namespace CxxAbb */


 

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
 * MutexImpl.cpp
 *
 * FileId      : $Id: MutexImpl.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Mar 8, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : POSIX Mutex Implementation
 *
 */

#include "MutexImpl.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h> // posix defines
#include <sys/time.h> // *nix specific

namespace CxxAbb
{
namespace Sys
{

MutexImpl::MutexImpl()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

	int ret = pthread_mutex_init(&t_Handle, &attr);

	if (ret != 0) throw SystemException("pthread_mutex_init failed");
}

MutexImpl::MutexImpl(int /*_NotUsed*/)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	int ret = pthread_mutex_init(&t_Handle, &attr);

	if (ret != 0) throw SystemException("pthread_mutex_init failed");
}

MutexImpl::~MutexImpl()
{
	pthread_mutex_destroy(&t_Handle);
}

void MutexImpl::LockImpl()
{
	if (pthread_mutex_lock(&t_Handle) != 0) throw SystemException("pthread_mutex_lock failed");
}

bool MutexImpl::TryLockImpl()
{
	int rc = pthread_mutex_trylock(&t_Handle);

	if (rc != 0 && rc != EBUSY) throw SystemException("pthread_mutex_trylock failed");

	return rc != EBUSY;
}

bool MutexImpl::TryLockImpl(long _lMiliSeconds)
{
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS - 200112L) >= 0L
#if defined(_POSIX_THREADS) && (_POSIX_THREADS - 200112L) >= 0L

	struct timespec abstime;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + _lMiliSeconds / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (_lMiliSeconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
	int rc = pthread_mutex_timedlock(&t_Handle, &abstime);
	if (rc == 0)
		return true;
	else if (rc == ETIMEDOUT)
		return false;
	else
		throw SystemException("cannot lock mutex");

#else
#error "NotImplemented: TryLockImpl with timeout"
	throw NotImplementedException("TryLockImpl with timeout");
#endif

#else
#error "_POSIX_TIMEOUTS Not defined"
	throw NotImplementedException("TryLockImpl with timeout");
#endif
}

void MutexImpl::UnlockImpl()
{
	if (pthread_mutex_unlock(&t_Handle) != 0) throw SystemException("pthread_mutex_unlock failed");
}

} /* namespace Sys */
} /* namespace CxxAbb */


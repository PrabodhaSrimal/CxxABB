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
 * MutexImpl.h
 *
 * FileId      : $Id: MutexImpl.h 20 2012-11-22 07:46:58Z prabodar $
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

#ifndef CXXABB_CORE_MUTEXIMPL_H_
#define CXXABB_CORE_MUTEXIMPL_H_

#include <CxxAbb/Core.h>
#include <pthread.h>

namespace CxxAbb
{
namespace Sys
{

class MutexImpl
{
public:
	explicit MutexImpl();

	MutexImpl(int n);

	virtual ~MutexImpl();

	void LockImpl();

	bool TryLockImpl();

	bool TryLockImpl(long _lMiliSeconds);

	void UnlockImpl();

	pthread_mutex_t * Handle()
	{
		return &t_Handle;
	}

	const pthread_mutex_t* Handle() const
	{
		return &t_Handle;
	}

private:
	pthread_mutex_t t_Handle;
};

class FastMutexImpl : public MutexImpl
{
public:
	FastMutexImpl() : MutexImpl(1)
	{}

	~FastMutexImpl()
	{}
};

} /* namespace Sys */
} /* namespace CxxAbb */



#endif /* CXXABB_CORE_MUTEXIMPL_H_ */

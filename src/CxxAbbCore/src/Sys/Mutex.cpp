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
 * Mutex.cpp
 *
 * FileId      : $Id: Mutex.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Mar 8, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Mutex and Locks
 *
 */

#include "MutexImpl.h"
#include <CxxAbb/Sys/Mutex.h>

namespace CxxAbb
{
namespace Sys
{

Mutex::Mutex() : MutexImpl()
{

}

Mutex::~Mutex()
{

}

void Mutex::Lock()
{
	LockImpl();
}

bool Mutex::TryLock()
{
	return TryLockImpl();
}

bool Mutex::TryLock(long _lMiliSeconds)
{
	return TryLockImpl(_lMiliSeconds);
}

void Mutex::Unlock()
{
	UnlockImpl();
}

void Mutex::UnlockNoThrow()
{
	try
	{
		UnlockImpl();
	}
	catch(...)
	{

	}
}


/// FastMutex

FastMutex::FastMutex() : FastMutexImpl()
{

}

FastMutex::~FastMutex()
{

}

void FastMutex::Lock()
{
	LockImpl();
}

bool FastMutex::TryLock()
{
	return TryLockImpl();
}

bool FastMutex::TryLock(long _lMiliSeconds)
{
	return TryLockImpl(_lMiliSeconds);
}

void FastMutex::Unlock()
{
	UnlockImpl();
}

void FastMutex::UnlockNoThrow()
{
	try
	{
		UnlockImpl();
	}
	catch(...)
	{

	}
}

} /* namespace Sys */
} /* namespace CxxAbb */

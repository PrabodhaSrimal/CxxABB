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
 * Mutex.h
 *
 * FileId      : $Id: Mutex.h 20 2012-11-22 07:46:58Z prabodar $
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

#ifndef CXXABB_CORE_MUTEX_H_
#define CXXABB_CORE_MUTEX_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include "MutexImpl.h"

namespace CxxAbb
{
namespace Sys
{


/** @brief A Mutual Exclusion Object
 *
 * Underlying implementation may vary on how to perform API methods
 */
class CXXABB_API Mutex : public NonCopyable, private MutexImpl
{
public:
	typedef CxxAbb::Sys::ScopedLock<Mutex> ScopedLock;

	Mutex(); // Throws SystemException if Initialization failed
	~Mutex();

	void Lock();
	bool TryLock();
	bool TryLock(long _lMiliSeconds);
	void Unlock();
	void UnlockNoThrow();

private:
};


/** @brief A Mutual Exclusion Object for high performance execution
 *
 * Underlying implementation may vary on how to perform API methods
 */
class CXXABB_API FastMutex : public NonCopyable, private FastMutexImpl
{
public:
	typedef CxxAbb::Sys::ScopedLock<FastMutex> ScopedLock;

	FastMutex();
	~FastMutex();

	void Lock();
	bool TryLock();
	bool TryLock(long _lMiliSeconds);
	void Unlock();
	void UnlockNoThrow();
};


} /* namespace Sys */
} /* namespace CxxAbb */

#endif /* CXXABB_CORE_MUTEX_H_ */

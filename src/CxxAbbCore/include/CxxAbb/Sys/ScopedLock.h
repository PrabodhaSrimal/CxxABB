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
 * ScopedLock.h
 *
 * FileId      : $Id: ScopedLock.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Mar 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Locking based on scope
 *
 */

#ifndef CXXABB_CORE_SCOPEDLOCK_H_
#define CXXABB_CORE_SCOPEDLOCK_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>

namespace CxxAbb
{
namespace Sys
{

/** @brief Automatic locking and unlocking mechanism for scoped code blocks
 *
 * Should provide a Locking Object (i.e. Mutex) that implement Locking API
 * void Lock(), void Unlock()
 */
template <class MutexClass>
class CXXABB_API ScopedLock : public NonCopyable
{
public:
	explicit ScopedLock(MutexClass& _mutex): m_Mutex(_mutex)
	{
		m_Mutex.Lock();
	}

	ScopedLock(MutexClass& _mutex, long _lMilliseconds): m_Mutex(_mutex)
	{
		m_Mutex.Lock(_lMilliseconds);
	}

	~ScopedLock()
	{
		m_Mutex.Unlock();
	}

	void Unlock()
	{
		m_Mutex.Unlock();
	}

private:
	ScopedLock();

	MutexClass & m_Mutex;
};

} /* namespace Sys */
} /* namespace CxxAbb */



#endif /* CXXABB_CORE_SCOPEDLOCK_H_ */

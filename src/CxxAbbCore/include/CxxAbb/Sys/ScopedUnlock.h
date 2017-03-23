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
 * ScopedUnlock.h
 *
 * FileId      : $Id: ScopedUnlock.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Unlocking based on scope exit
 *
 */

#ifndef CXXABB_CORE_SCOPEDUNLOCK_H_
#define CXXABB_CORE_SCOPEDUNLOCK_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>

namespace CxxAbb
{

namespace Sys
{

/** @brief Automatic unlocking and locking mechanism for scoped code blocks
 *
 * Should provide a Locking Object (i.e. Mutex) that implement Locking API
 * void Lock(), void Unlock()
 */
template <class MutexClass>
class CXXABB_API ScopedUnlock : public NonCopyable
{
public:
	explicit ScopedUnlock(MutexClass& _mutex, bool _bImmidiate = true): m_Mutex(_mutex)
	{
		if(_bImmidiate)
			m_Mutex.Unlock();
	}

	~ScopedUnlock()
	{
		m_Mutex.Lock();
	}

	void Unlock()
	{
		m_Mutex.Unlock();
	}

	void Lock()
	{
		m_Mutex.Lock();
	}

private:
	ScopedUnlock();

	MutexClass & m_Mutex;
};

}  /* namespace Sys */

}  /* namespace CxxAbb */



#endif /* CXXABB_CORE_SCOPEDUNLOCK_H_ */

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
 * SigEventImpl.h
 *
 * FileId      : $Id: SigEventImpl.h 20 2012-11-22 07:46:58Z prabodar $
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

#ifndef CXXABB_CORE_SIGEVENTIMPL_H_
#define CXXABB_CORE_SIGEVENTIMPL_H_

#include <CxxAbb/Core.h>

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API SigEventImpl
{
public:
	SigEventImpl(bool _autoreset);
	virtual ~SigEventImpl();

protected:
	void SetImpl();
	void WaitImpl();
	bool TryWaitImpl(long _lMiliSeconds);
	void ResetImpl();

private:
	bool b_AutoReset;
	volatile bool b_EventHappened;
	pthread_mutex_t mtx_Lock; // Lock is for EventState
	pthread_cond_t cnd_Cond;
};

inline void SigEventImpl::SetImpl()
{
	// get lock
	if (pthread_mutex_lock(&mtx_Lock))
		throw SystemException("pthread_mutex_lock failed");

	// set event
	b_EventHappened = true;

	// notify waiters
	if (pthread_cond_broadcast(&cnd_Cond))
	{
		pthread_mutex_unlock(&mtx_Lock);
		throw SystemException("pthread_mutex_lock failed");
	}

	// release lock
	pthread_mutex_unlock(&mtx_Lock);
}

inline void SigEventImpl::ResetImpl()
{
	if (pthread_mutex_lock(&mtx_Lock))
		throw SystemException("pthread_mutex_lock failed");
	b_EventHappened = false;
	pthread_mutex_unlock(&mtx_Lock);
}

}  /* namespace Sys */

}  /* namespace CxxAbb */

#endif /* CXXABB_CORE_SIGEVENTIMPL_H_ */

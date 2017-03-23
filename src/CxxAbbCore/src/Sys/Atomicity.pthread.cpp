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
 * Atomicity.pthread.cpp
 *
 * FileId      : $Id: Atomicity.pthread.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 4, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Atomic operations using pthread
 *
 */


#include <CxxAbb/Sys/Atomicity.h>
#include <pthread.h>

namespace CxxAbb
{
namespace Sys
{

typedef void(*cleanup_proc_t)(void *);

static pthread_mutex_t S_Mutex = PTHREAD_MUTEX_INITIALIZER;


AtomicCounter::AtomicCounter()
		: t_Counter(0)
{
}

AtomicCounter::AtomicCounter(const AtomicCounter& _counter)
		: t_Counter(_counter.Value())
{
}

AtomicCounter::AtomicCounter(AtomicType _value)
		: t_Counter(_value)
{
}

AtomicCounter::AtomicCounter(unsigned int _value)
	: t_Counter(_value)
{
}

AtomicCounter::~AtomicCounter()
{
}

inline AtomicCounter::operator AtomicType() const
{
	int iRet = 0;

	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	iRet = t_Counter;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return iRet;
}

inline AtomicType AtomicCounter::Value() const
{
	AtomicType t;

	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	t = t_Counter;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return t;
}

/// Operators
inline AtomicCounter& AtomicCounter::operator =(const AtomicCounter& _counter)
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	t_Counter = _counter.Value();

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline AtomicCounter& AtomicCounter::operator =(AtomicType _atomictype)
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	t_Counter = _atomictype;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline AtomicType AtomicCounter::operator ++()
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	t_Counter++;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline AtomicType AtomicCounter::operator ++(Int32)
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	++t_Counter;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline AtomicType AtomicCounter::operator --()
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	t_Counter--;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline AtomicType AtomicCounter::operator --(Int32)
{
	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	--t_Counter;

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return *this;
}

inline bool AtomicCounter::operator !() const
{
	bool bRet = false;

	pthread_cleanup_push((cleanup_proc_t)pthread_mutex_unlock, (void *)&S_Mutex);
	pthread_mutex_lock(&S_Mutex);

	bRet = (t_Counter == 0);

	pthread_mutex_unlock(&S_Mutex);
	pthread_cleanup_pop(0);

	return bRet;
}

} /* namespace Sys */
} /* namespace CxxAbb */



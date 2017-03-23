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
 * WaitCondition.cpp
 *
 * FileId      : $Id: WaitCondition.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 18, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Condition which thread can wait and/or signal
 *
 */

#include <CxxAbb/Sys/WaitCondition.h>

namespace CxxAbb
{

namespace Sys
{

void WaitCondition::Signal()
{
	CxxAbb::Sys::FastMutex::ScopedLock lckQueue(mtx_Queue);

	if(!m_WaitQueue.empty())
	{
		m_WaitQueue.front()->Set();
		m_WaitQueue.pop_back();
	}
}

void WaitCondition::SignalAll()
{
	CxxAbb::Sys::FastMutex::ScopedLock lckQueue(mtx_Queue);

	for (EventQueue::iterator ite = m_WaitQueue.begin(); ite != m_WaitQueue.end(); ++ite)
	{
		(*ite)->Set();
	}
	m_WaitQueue.clear();
}

void WaitCondition::RemoveEvent(CxxAbb::Sys::SigEvent & _evt)
{
	for (EventQueue::iterator ite = m_WaitQueue.begin(); ite != m_WaitQueue.end(); ++ite)
	{
		if (*ite == &_evt)
		{
			m_WaitQueue.erase(ite);
			break;
		}
	}
}

}  /* namespace Sys */

}  /* namespace CxxAbb */



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
 * SigEvent.h
 *
 * FileId      : $Id: SigEvent.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 12, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Threading
 * Comment     : Signaling Event for synchronization waiting on a WaitCondition
 *
 */

#ifndef CXXABB_CORE_SIGEVENT_H_
#define CXXABB_CORE_SIGEVENT_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include "SigEventImpl.h"

namespace CxxAbb
{

namespace Sys
{

/** @brief SigEvent is used to signal one or more waiting threads on a synchronization-wait-
 *  condition about some event that has happened.
 */
class CXXABB_API SigEvent : public CxxAbb::NonCopyable, private SigEventImpl
{
public:
	SigEvent(bool _autoreset = true);
	~SigEvent();

	/** @brief Set this event has happened
	 */
	void Set();
	/** @brief Wait for this event to happen
	 */
	void Wait();
	/** @brief Wait for this event at most x milliseconds
	 */
	bool TryWait(long _lMiliSeconds);
	/** @brief Reset event - i.e. event has not happened
	 */
	void Reset();
};

}  /* namespace Sys */

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_SIGEVENT_H_ */

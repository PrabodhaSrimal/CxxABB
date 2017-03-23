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
 * SigEvent.cpp
 *
 * FileId      : $Id: SigEvent.cpp 20 2012-11-22 07:46:58Z prabodar $
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

#include <CxxAbb/Sys/SigEvent.h>

namespace CxxAbb
{

namespace Sys
{

SigEvent::SigEvent(bool _autoreset) : SigEventImpl(_autoreset)
{
}

SigEvent::~SigEvent()
{
}

void SigEvent::Set()
{
	SetImpl();
}

void SigEvent::Wait()
{
	WaitImpl();
}

bool SigEvent::TryWait(long _lMiliSeconds)
{
	return TryWaitImpl(_lMiliSeconds);
}

void SigEvent::Reset()
{
	ResetImpl();
}

}  /* namespace Sys */

}  /* namespace CxxAbb */
 

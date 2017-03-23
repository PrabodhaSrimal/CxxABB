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
 * Runnable.h
 *
 * FileId      : $Id: Runnable.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 18, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Runnable interface to thread class
 *
 */

#ifndef CXXABB_CORE_RUNNABLE_H_
#define CXXABB_CORE_RUNNABLE_H_

#include <CxxAbb/Core.h>

namespace CxxAbb
{

/** @brief Runnable interface
 *  subclasses should implement run method
 */
class CXXABB_API Runnable
{
public:
	virtual ~Runnable() {}

	virtual void Run() = 0;

};

}  /* namespace CxxAbb */

#endif /* CXXABB_CORE_RUNNABLE_H_ */

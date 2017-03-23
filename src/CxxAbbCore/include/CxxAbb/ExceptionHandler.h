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
 * ExceptionHandler.h
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 21, 2012
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Global Exception Handler
 *
 */

#ifndef CXXABB_CORE_EXCEPTIONHANDLER_H_
#define CXXABB_CORE_EXCEPTIONHANDLER_H_


#include <CxxAbb/Core.h>
#include <CxxAbb/Exception.h>

namespace CxxAbb
{

/** @brief Global Exception Handler Base
 *  Custom exception handlers should derive from this class
 *  and must provide implementations for Handle() functions.
 *  Default behaviour is to log and ignore exception
 */
class CXXABB_API ExceptionHandler
{
public:
	ExceptionHandler()
	{}

	virtual ~ExceptionHandler()
	{}

	virtual void Exception(const CxxAbb::Exception & _e);

	virtual void Exception(const std::exception & _e);

	virtual void Exception();

};

/** @brief Thread Exception Handler
 *  Thread function thrown exceptions are handle by this class and its registerd handlers
 *  Custom handlers can be registered with Set() function
 */
class CXXABB_API ThreadErrorHandler
{
public:
	ThreadErrorHandler()
	{}

	~ThreadErrorHandler()
	{}

	static void Handle(const CxxAbb::Exception & _e);

	static void Handle(const std::exception & _e);

	static void Handle();

	/** @brief Set custom exception handler derived from CxxAbb::ExceptionHandler
	 *  Function will return existing handler
	 */
	static ExceptionHandler * Set(ExceptionHandler * _new);

	static ExceptionHandler * Get();

private:
	static ExceptionHandler * p_CurrentHandler;
	static CxxAbb::Sys::FastMutex m_Mutex;
};

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_EXCEPTIONHANDLER_H_ */

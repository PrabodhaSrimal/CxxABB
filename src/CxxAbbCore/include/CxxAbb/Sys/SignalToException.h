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
 * SignalToException.h
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 21, 2012
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : CxxAbbCore
 * Module      : System
 * Comment     : POSIX signal to C++ Exception translater
 *
 */

#ifndef CXXABB_CORE_SIGNALTOEXCEPTION_H_
#define CXXABB_CORE_SIGNALTOEXCEPTION_H_

#include <CxxAbb/Core.h>

/// POSIX signals are only available in UNIX
#if defined(CXXABB_OS_FAMILY_UNIX)

#include <vector>
#include <setjmp.h>

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API ThreadImpl;

class CXXABB_API  SignalToException
{
public:
	typedef void (*SigHandlerFunc)(int);
	typedef struct JumpBuffer
	{
		sigjmp_buf m_Env;
	} JumpBuffer;
	typedef std::vector<JumpBuffer> JumpBufferVec;

	SignalToException();
	~SignalToException();

	static void SetupHandler(int _iSigNum = -1,
		SigHandlerFunc _handler = &SignalToException::SignalHandler);

	static void ThrowException(int _iSigNum);

	sigjmp_buf& CurrentEnv();

protected:
	static void SignalHandler(int _iSigNum);

	static JumpBufferVec& GetJumpBuffers();

private:

	static JumpBufferVec m_JumpBuffers;
	friend class CxxAbb::Sys::ThreadImpl;
};

#define ThrowOnSignal() \
	CxxAbb::Sys::SignalToException _mSignalToException; \
	int _sig_ret = sigsetjmp(_mSignalToException.CurrentEnv(), 1); \
	if (_sig_ret) _mSignalToException.ThrowException(_sig_ret);

}  /* namespace Sys */

}  /* namespace CxxAbb */

#endif

#endif /* CXXABB_CORE_SIGNALTOEXCEPTION_H_ */

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
 * SignalToException.cpp
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

#include <CxxAbb/Sys/SignalToException.h>
#include <CxxAbb/Sys/Thread.h>
#include <signal.h>

#if defined(CXXABB_OS_FAMILY_UNIX)

namespace CxxAbb
{

namespace Sys
{

SignalToException::JumpBufferVec SignalToException::m_JumpBuffers;

SignalToException::SignalToException()
{
	JumpBufferVec& bufs = GetJumpBuffers();
	JumpBuffer buf;
	bufs.push_back(buf);
}

SignalToException::~SignalToException()
{
	GetJumpBuffers().pop_back();
}

SignalToException::JumpBufferVec& SignalToException::GetJumpBuffers()
{
	CxxAbb::Sys::ThreadImpl * pCurrThread = CxxAbb::Sys::ThreadImpl::CurrentImpl();
	if(pCurrThread)
	{
		// in a thread
		return pCurrThread->m_JumpBuffers;
	}
	else
	{
		// in process's main thread
		return m_JumpBuffers;
	}
}

sigjmp_buf& SignalToException::CurrentEnv()
{
	return GetJumpBuffers().back().m_Env;
}

void SignalToException::SetupHandler(int _iSigNum, SigHandlerFunc _handler)
{
	if(_iSigNum > 0)
	{
		struct sigaction sa;
		sa.sa_handler = _handler;
		sa.sa_flags   = SA_RESETHAND;
		sigemptyset(&sa.sa_mask);
		sigaction(_iSigNum,  &sa, 0);
	}
	else
	{
		struct sigaction sa;
		sa.sa_handler = _handler;
		sa.sa_flags   = SA_RESETHAND;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGILL,  &sa, 0);
		sigaction(SIGBUS,  &sa, 0);
		sigaction(SIGFPE,  &sa, 0);
		sigaction(SIGSEGV,  &sa, 0);
		sigaction(SIGSYS,  &sa, 0);
	}
}

void SignalToException::SignalHandler(int _iSigNum)
{
	JumpBufferVec& jb = GetJumpBuffers();
	if (!jb.empty())
		siglongjmp(jb.back().m_Env, _iSigNum);

	throw CxxAbb::SignalException("Unhandled Signal", _iSigNum);
	//std::abort();
}

void SignalToException::ThrowException(int _iSigNum)
{
	switch(_iSigNum)
	{
//		case SIGHUP	: //1	/* Hangup (POSIX).  */
//		case SIGINT: //2	/* Interrupt (ANSI).  */
//		case SIGQUIT: //3	/* Quit (POSIX).  */
		case SIGILL: //4	/* Illegal instruction (ANSI).  */
			throw CxxAbb::SignalException("Illegal instruction", _iSigNum);
//		case SIGTRAP: //5	/* Trace trap (POSIX).  */
//		case SIGABRT: //6	/* Abort (ANSI).  */
//		case SIGIOT: //6	/* IOT trap (4.2 BSD).  */
		case SIGBUS: //7	/* BUS error (4.2 BSD).  */
			throw CxxAbb::SignalException("BUS error", _iSigNum);
		case SIGFPE: //8	/* Floating-point exception (ANSI).  */
			throw CxxAbb::SignalException("Floating-point exception", _iSigNum);
//		case SIGKILL: //9	/* Kill, unblockable (POSIX).  */
//		case SIGUSR1: //10	/* User-defined signal 1 (POSIX).  */
		case SIGSEGV: //11	/* Segmentation violation (ANSI).  */
			throw CxxAbb::SignalException("Segmentation violation", _iSigNum);
//		case SIGUSR2: //12	/* User-defined signal 2 (POSIX).  */
//		case SIGPIPE: //13	/* Broken pipe (POSIX).  */
//		case SIGALRM: //14	/* Alarm clock (POSIX).  */
//		case SIGTERM: //15	/* Termination (ANSI).  */
//		case SIGSTKFLT:	// 16	/* Stack fault.  */
//		case SIGCLD: //SIGCHLD	/* Same as SIGCHLD (System V).  */
//		case SIGCHLD: //17	/* Child status has changed (POSIX).  */
//		case SIGCONT: //18	/* Continue (POSIX).  */
//		case SIGSTOP: //19	/* Stop, unblockable (POSIX).  */
//		case SIGTSTP: //20	/* Keyboard stop (POSIX).  */
//		case SIGTTIN: //21	/* Background read from tty (POSIX).  */
//		case SIGTTOU: //22	/* Background write to tty (POSIX).  */
//		case SIGURG: //23	/* Urgent condition on socket (4.2 BSD).  */
//		case SIGXCPU: //24	/* CPU limit exceeded (4.2 BSD).  */
//		case SIGXFSZ: //25	/* File size limit exceeded (4.2 BSD).  */
//		case SIGVTALRM:	// 26	/* Virtual alarm clock (4.2 BSD).  */
//		case SIGPROF: //27	/* Profiling alarm clock (4.2 BSD).  */
//		case SIGWINCH:	// 28	/* Window size change (4.3 BSD, Sun).  */
//		case SIGPOLL: //SIGIO	/* Pollable event occurred (System V).  */
//		case SIGIO: //29	/* I/O now possible (4.2 BSD).  */
//		case SIGPWR: //30	/* Power failure restart (System V).  */
		case SIGSYS: //31	/* Bad system call.  */
			throw CxxAbb::SignalException("Bad system call", _iSigNum);
		default:
			break;
	}
}

}  /* namespace Sys */

}  /* namespace CxxAbb */

#endif

 

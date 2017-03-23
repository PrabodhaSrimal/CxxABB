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
 * EnvironmentImpl.h
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Jan 2, 2013
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : CxxAbbCore
 * Module      : System
 * Comment     : Application Running Environment - POSIX implimentation
 *
 */

#ifndef CXXABB_CORE_ENVIRONMENTIMPL_H_
#define CXXABB_CORE_ENVIRONMENTIMPL_H_


#include <CxxAbb/Core.h>
#include <CxxAbb/Sys/Mutex.h>

namespace CxxAbb
{

namespace Sys
{

class CXXABB_API EnvironmentImpl
{
public:
	typedef UInt8 NodeId[6];

	static std::string GetImpl(const std::string & _key);

	static std::string GetImpl(const std::string & _key, const std::string & _default);

	static bool HasImpl(const std::string & _key);

	static void SetImpl(const std::string & _key, const std::string & _value);

	static std::string OsNameImpl();

	static std::string HostNameImpl();

	static void EthernetAddressImpl(NodeId & _address);

	static std::string OsVersionImpl();

	static std::string OsArchitectureImpl();

	static unsigned int ProcessorCountImpl();
private:

	static CxxAbb::Sys::FastMutex m_Mutex;
};

}  /* namespace Sys */

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_ENVIRONMENTIMPL_H_ */

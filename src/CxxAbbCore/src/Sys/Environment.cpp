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
 * Environment.cpp
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
 * Comment     : Application Running Environment
 *
 */


#include <CxxAbb/Sys/Environment.h>
#include "EnvironmentImpl.h"
#include <CxxAbb/Version.h>
#include <sstream>
#include <iomanip>

namespace CxxAbb
{

namespace Sys
{

std::string Environment::Get(const std::string & _key)
{
	return EnvironmentImpl::GetImpl(_key);
}

std::string Environment::Get(const std::string & _key, const std::string & _default)
{
	return EnvironmentImpl::GetImpl(_key, _default);
}

bool Environment::Has(const std::string & _key)
{
	return EnvironmentImpl::HasImpl(_key);
}

void Environment::Set(const std::string & _key, const std::string & _value)
{
	EnvironmentImpl::SetImpl(_key, _value);
}

std::string Environment::OsName()
{
	return EnvironmentImpl::OsNameImpl();
}

std::string Environment::HostName()
{
	return EnvironmentImpl::HostNameImpl();
}

std::string Environment::EthernetAddress()
{
	NodeId id;
	EnvironmentImpl::EthernetAddressImpl(id);
	char result[18];
	std::sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
		id[0],
		id[1],
		id[2],
		id[3],
		id[4],
		id[5]);
	return std::string(result);
}

void Environment::EthernetAddress(Environment::NodeId & _address)
{
	EnvironmentImpl::EthernetAddressImpl(_address);
}

std::string Environment::OsVersion()
{
	return EnvironmentImpl::OsVersionImpl();
}

std::string Environment::OsArchitecture()
{
	return EnvironmentImpl::OsArchitectureImpl();
}

unsigned int Environment::ProcessorCount()
{
	return EnvironmentImpl::ProcessorCountImpl();
}

std::string Environment::LibraryVersionString()
{
	std::stringstream ver;
	ver << LibraryMajorVersion();
	ver << ".";
	ver << std::setfill('0') << std::setw(2) << LibraryMinorVersion();
	ver << ".";
	ver << std::setfill('0') << std::setw(4) << LibraryRevisionVersion();
	return ver.str();
}

int Environment::LibraryVersion()
{
	return CXXABB_VERSION;
}

int Environment::LibraryMajorVersion()
{
	return (CXXABB_VERSION & 0xFF000000) >> 24;
}

int Environment::LibraryMinorVersion()
{
	return (CXXABB_VERSION & 0x00FF0000) >> 16;
}

int Environment::LibraryRevisionVersion()
{
	return (CXXABB_VERSION & 0x0000FFFF);
}


}  /* namespace Sys */

}  /* namespace CxxAbb */

 

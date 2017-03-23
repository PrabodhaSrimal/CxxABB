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
 * Environment.h
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

#ifndef CXXABB_CORE_ENVIRONMENT_H_
#define CXXABB_CORE_ENVIRONMENT_H_

#include <CxxAbb/Core.h>

namespace CxxAbb
{

namespace Sys
{

/** @brief System independent environment information accessor
 *  Gives access to Environment variables, System details, etc.
 */
class CXXABB_API Environment
{
public:
	typedef UInt8 NodeId[6]; /// Ethernet address.

	/** @brief Get environment variable by name. Throws if not found.
	 */
	static std::string Get(const std::string & _key);

	/** @brief Get environment variable by name. If not found return default.
	 */
	static std::string Get(const std::string & _key, const std::string & _default);

	/** @brief Check if environment variable is set
	 */
	static bool Has(const std::string & _key);

	/** @brief Set environment variable
	 */
	static void Set(const std::string & _key, const std::string & _value);

	/** @brief Get OS Name
	 */
	static std::string OsName();

	/** @brief Get Host Name
	 */
	static std::string HostName();

	/** @brief Get Ethernet Address as hex[] or string
	 */
	static std::string EthernetAddress();
	static void EthernetAddress(NodeId & _address);

	/** @brief Get OS release version
	 */
	static std::string OsVersion();

	/** @brief Get OS Architecture
	 */
	static std::string OsArchitecture();

	/** @brief Get system process count
	 */
	static unsigned int ProcessorCount();

	/** @brief Get CxxAbb library version
	 */
	static std::string LibraryVersionString();
	static int LibraryVersion();
	static int LibraryMajorVersion();
	static int LibraryMinorVersion();
	static int LibraryRevisionVersion();

private:
	Environment() {}
	~Environment() {}

	void operator=(const Environment);
};

}  /* namespace Sys */

}  /* namespace CxxAbb */



#endif /* CXXABB_CORE_ENVIRONMENT_H_ */

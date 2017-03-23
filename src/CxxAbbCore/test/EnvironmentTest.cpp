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
 * EnvironmentTest.cpp
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Jan 2, 2013
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */


#include <CxxAbb/Sys/Environment.h>
#include <gtest/gtest.h>


TEST(EnvironmentTest, Details)
{
	COUT_LOG() << "CxxAbb Version:  " << CxxAbb::Sys::Environment::LibraryVersionString();
	COUT_LOG() << "OS Name:         " << CxxAbb::Sys::Environment::OsName();
	COUT_LOG() << "OS Version:      " << CxxAbb::Sys::Environment::OsVersion();
	COUT_LOG() << "OS Architecture: " << CxxAbb::Sys::Environment::OsArchitecture();
	COUT_LOG() << "Node Name:       " << CxxAbb::Sys::Environment::HostName();
	COUT_LOG() << "Node ID:         " << CxxAbb::Sys::Environment::EthernetAddress();
	COUT_LOG() << "Number of CPUs:  " << CxxAbb::Sys::Environment::ProcessorCount();
}

TEST(EnvironmentTest, Variables)
{
	CxxAbb::Sys::Environment::Set("MY_TESTING_ENV", "MY_TESTING_VAL");
	ASSERT_TRUE(CxxAbb::Sys::Environment::Has("MY_TESTING_ENV"));
	ASSERT_TRUE(CxxAbb::Sys::Environment::Get("MY_TESTING_ENV").compare("MY_TESTING_VAL") == 0);

	if (!CxxAbb::Sys::Environment::Has("NON_EXISTING_ENV_VAR"))
	{
		ASSERT_THROW(CxxAbb::Sys::Environment::Get("NON_EXISTING_ENV_VAR"),
			CxxAbb::NotFoundException);
		ASSERT_NO_THROW(CxxAbb::Sys::Environment::Get("NON_EXISTING_ENV_VAR",
			"NON_EXISITING_VALUE"));
		std::string nev = CxxAbb::Sys::Environment::Get("NON_EXISTING_ENV_VAR",
			"NON_EXISITING_VALUE");

		ASSERT_TRUE(nev.compare("NON_EXISITING_VALUE") == 0);
	}

	std::string sPath = CxxAbb::Sys::Environment::Get("PATH", "No PATH Environment set");
	COUT_LOG() << "Path Env : " << sPath;
}

 

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
 * NullType.h
 *
 * FileId      : $Id: NullType.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 22, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Empty type to denote Null
 *
 */

#ifndef CXXABB_CORE_NULLTYPE_H_
#define CXXABB_CORE_NULLTYPE_H_

#include <CxxAbb/Core.h>
#include <typeinfo>

namespace CxxAbb
{

/// Empty type to denote Null

class CXXABB_API Null
{

};

/// Extractred from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr
const class NullPtrT
{
public:
	template<class T>
	inline operator T*() const // convertible to any type of null non-member pointer...
	{ return 0; }

	template<class C, class T>
	inline operator T C::*() const   // or any type of null member pointer...
	{ return 0; }

private:
	void operator&() const;  // Can't take address of nullptr

} NullPtr = {};

//#define NullPtr NULL

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_NULLTYPE_H_ */

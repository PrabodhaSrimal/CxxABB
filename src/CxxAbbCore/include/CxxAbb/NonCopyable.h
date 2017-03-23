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
 * NonCopyable.h
 *
 * FileId      : $Id: NonCopyable.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 22, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Policy
 * Comment     : Non-Copyable interface
 *
 */

#ifndef CXXABB_CORE_NONCOPYABLE_H_
#define CXXABB_CORE_NONCOPYABLE_H_


#include <CxxAbb/Core.h>

namespace CxxAbb
{

/** @brief Makes derived classes cannot be copied
 *
 * Trying to do so will yield a compile-time error
 * Should use as..
 *
 * @code
 * class DerivedClass : private NonCopyable { }
 * @endcode
 *
 */
class CXXABB_API NonCopyable
{
public:
	NonCopyable()
	{}

	~NonCopyable() // Not virtual
	{}

private:
	/// Private Copy constructor
	NonCopyable(const NonCopyable&);

	/// Private assignment operator
	NonCopyable & operator=(const NonCopyable&);
};

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_NONCOPYABLE_H_ */

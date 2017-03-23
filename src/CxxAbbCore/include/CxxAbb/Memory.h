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
 * Memory.h
 *
 * FileId      : $Id: Memory.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 23, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Memory
 * Comment     : Smart pointer implementation includes
 *
 */

#ifndef CXXABB_CORE_MEMORY_H_
#define CXXABB_CORE_MEMORY_H_


#include <cstddef> // __GLIBCXX__, _HAS_TR1

// GNU C++ or Intel C++ using libstd++.
//
#if defined (__GNUC__) && __GNUC__ >= 4 && defined (__GLIBCXX__)
#  include <tr1/memory>

namespace CxxAbb
{
using namespace std::tr1;
}

//
// IBM XL C++.
//
#elif defined (__xlC__) && __xlC__ >= 0x0900
#  define __IBMCPP_TR1__
#  include <memory>
//
// VC++ or Intel C++ using VC++ standard library.
//
#elif defined (_MSC_VER) && (_MSC_VER == 1500 && defined (_HAS_TR1) || _MSC_VER > 1500)
#  include <memory>
//
// fall-back to our implementation
//
#else

  #include <CxxAbb/Atomicity.h>
  #include <CxxAbb/SharedPtr.h>
#endif



#endif /* CXXABB_CORE_MEMORY_H_ */

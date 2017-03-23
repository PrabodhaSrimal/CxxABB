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
 * Config.h
 *
 * FileId      : $Id: Config.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Oct 11, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Global Library Configurations
 *
 */

#ifndef CXXABB_CORE_CONFIG_H_
#define CXXABB_CORE_CONFIG_H_


/// Exceptions enabled globally
#define CXXABB_EXCEPIONS

/// Export Library API functions
//#define CXXABB_API_EXPORT

/// Debugging of library
#ifdef NDEBUG
#define CXXABB_DEBUG 1
#endif

//#define CXXABB_DEF_ATOMICITY_GCC_X86
//#define CXXABB_DEF_ATOMICITY_GCC_X86_64
//#define CXXABB_DEF_ATOMICITY_WINDOWS
//#define CXXABB_DEF_ATOMICITY_PTHREAD

#endif /* CXXABB_CORE_CONFIG_H_ */

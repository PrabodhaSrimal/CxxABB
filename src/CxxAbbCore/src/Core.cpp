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
 * Core.cpp
 *
 * FileId      : $Id: Core.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 13, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Core Includes
 *
 */


#include <CxxAbb/Core.h>
#include <CxxAbb/BitOps.h>
#include <CxxAbb/Buffer.h>
#include <CxxAbb/Cloneable.h>
#include <CxxAbb/Config.h>
#include <CxxAbb/Debug.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Memory.h>
#include <CxxAbb/NonCopyable.h>
#include <CxxAbb/NullType.h>
#include <CxxAbb/Platform.h>
#include <CxxAbb/Platform_POSIX.h>
#include <CxxAbb/Platform_WIN32.h>
#include <CxxAbb/RefCountedObj.h>
#include <CxxAbb/RefCountedObjPool.h>
#include <CxxAbb/Singleton.h>
#include <CxxAbb/SmartPtr.h>
#include <CxxAbb/SourceLineInfo.h>
#include <CxxAbb/SwapByteOrder.h>
#include <CxxAbb/TypeInfo.h>
#include <CxxAbb/Types.h>

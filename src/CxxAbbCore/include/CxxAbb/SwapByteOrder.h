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
 * SwapByteOrder.h
 *
 * FileId      : $Id: SwapByteOrder.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 21, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Byte order swapping helper functions
 *
 */

#ifndef CXXABB_CORE_SWAPBYTEORDER_H_
#define CXXABB_CORE_SWAPBYTEORDER_H_


#include <CxxAbb/Core.h>
#include <CxxAbb/Platform.h>

namespace CxxAbb
{

template <typename T>
inline T Swab16(T _value)
{
    return ( (_value & 0x00FF) << 8 ) |
           ( (_value & 0xFF00) >> 8 );
}

template <typename T>
inline T Swab32(T _value)
{
    return ( (_value & 0x000000FF) << 24 ) |
           ( (_value & 0x0000FF00) <<  8 ) |
           ( (_value & 0x00FF0000) >>  8 ) |
           ( (_value & 0xFF000000) >> 24 );
}

template <typename T>
inline T Swab64(T _value)
{
    return ( (_value & 0x00000000000000FFULL) << 56 ) |
           ( (_value & 0x000000000000FF00ULL) << 40 ) |
           ( (_value & 0x0000000000FF0000ULL) << 24 ) |
           ( (_value & 0x00000000FF000000ULL) <<  8 ) |
           ( (_value & 0x000000FF00000000ULL) >>  8 ) |
           ( (_value & 0x0000FF0000000000ULL) >> 24 ) |
           ( (_value & 0x00FF000000000000ULL) >> 40 ) |
           ( (_value & 0xFF00000000000000ULL) >> 56 );
}

inline Int8 Swab(Int8 _value)
{
	return _value;
}

inline UInt8 Swab(UInt8 _value)
{
	return _value;
}

inline Int16 Swab(Int16 _value)
{
	return Swab16(_value);
}

inline UInt16 Swab(UInt16 _value)
{
	return Swab16(_value);
}

inline Int32 Swab(Int32 _value)
{
	return Swab32(_value);
}

inline UInt32 Swab(UInt32 _value)
{
	return Swab32(_value);
}

#ifdef CXXABB_64_BIT_ENABLED

inline Int64 Swab(Int64 _value)
{
	return Swab64(_value);
}

inline UInt64 Swab(UInt64 _value)
{
	return Swab64(_value);
}

#endif

class CXXABB_API ByteOrder
{
public:

/// Runtime endianess check

static inline bool BigEndian()
{
    const int i = 1;
    return *reinterpret_cast<const Int8*>(&i) == 0;
}

static inline bool LittleEndian()
{
    const int i = 1;
    return *reinterpret_cast<const Int8*>(&i) == 1;
}

/// Conversion functions from Host Byte order to Network Byte Order
/// Network Byte order is BigEndian

template <typename T>
static inline T HostToLe(T _value)
{
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	return _value;
#else
	return Swab(_value);
#endif
}

template <typename T>
static inline T HostToBe(T _value)
{
#ifdef CXXABB_ARCH_BIG_ENDIAN
	return _value;
#else
	return Swab(_value);
#endif
}

template <typename T>
static inline T LeToHost(T _value)
{
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	return _value;
#else
	return Swab(_value);
#endif
}

template <typename T>
static inline T BeToHost(T _value)
{
#ifdef CXXABB_ARCH_BIG_ENDIAN
	return _value;
#else
	return Swab(_value);
#endif
}

template <typename T>
static inline T HostToNetwork(T _value)
{
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	return Swab(_value);
#else
	return _value;
#endif
}

template <typename T>
static inline T NetworkToHost(T _value)
{
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	return Swab(_value);
#else
	return _value;
#endif
}

};

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_SWAPBYTEORDER_H_ */

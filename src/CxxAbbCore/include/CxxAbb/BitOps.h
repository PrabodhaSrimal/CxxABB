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
 * BitOps.h
 *
 * FileId      : $Id: BitOps.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 22, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Bit operation helper macros
 *
 */

#ifndef CXXABB_CORE_BITOPS_H_
#define CXXABB_CORE_BITOPS_H_

#include <CxxAbb/Core.h>
#include <ostream>

namespace CxxAbb
{

/// Bit wise operation macros
/// Make sure flag and mask are same size

/* Create a mask with n th bit set */
#define BIT(n)								( 1 << (n) )

/* set each bit defined by mask on flag */
#define BITOP_SET_MASK(flag, mask)			(flag |= mask)

/* clear each bit defined by mask on flag */
#define BITOP_CLEAR_MASK(flag, mask)		(flag &= ~(mask))

/* toggle each bit defined by mask on flag */
#define BITOP_TOGGLE_MASK(flag, mask)		(flag ^= mask)

/* check if any of bit defined by mask is set on flag */
#define BITOP_IS_ANY_SET_MASK(flag, mask)	((flag & mask) > 0)

/* set flag to zero */
#define BITOP_RESET_MASK(flag)				(flag = 0x00)


/// following macros are based on work by <Peteris Krumins>
/// http://www.catonmat.net/blog/bit-hacks-header-file

#define HEXIFY(X) 0x##X##LU

#define B8FY(Y) (((Y&0x0000000FLU)?1:0)  + \
                  ((Y&0x000000F0LU)?2:0)  + \
                  ((Y&0x00000F00LU)?4:0)  + \
                  ((Y&0x0000F000LU)?8:0)  + \
                  ((Y&0x000F0000LU)?16:0) + \
                  ((Y&0x00F00000LU)?32:0) + \
                  ((Y&0x0F000000LU)?64:0) + \
                  ((Y&0xF0000000LU)?128:0))

/// turns Z written in binary into decimal,
/// B8(11001101)
#define B8(Z) ((unsigned char)B8FY(HEXIFY(Z)))

/* test if x is even */
#define BITOP_IS_EVEN(x)        (((x)&1)==0)

/* test if x is odd */
#define BITOP_IS_ODD(x)         (!BITOP_IS_EVEN((x)))

/* test if n-th bit in x is set */
#define BITOP_IS_SET(x, n)   ((x) & (1<<(n)))

/* set n-th bit in x */
#define BITOP_SET(x, n)      ((x) |= (1<<(n)))

/* unset n-th bit in x */
#define BITOP_UNSET(x, n)    ((x) &= ~(1<<(n)))

/* toggle n-th bit in x */
#define BITOP_TOGGLE(x, n)   ((x) ^= (1<<(n)))

/* turn off right-most 1-bit in x */
#define BITOP_TURNOFF_1(x)   ((x) &= ((x)-1))

/* isolate right-most 1-bit in x */
#define BITOP_ISOLATE_1(x)   ((x) &= (-(x)))

/* right-propagate right-most 1-bit in x */
#define BITOP_PROPAGATE_1(x) ((x) |= ((x)-1))

/* isolate right-most 0-bit in x */
#define BITOP_ISOLATE_0(x)   ((x) = ~(x) & ((x)+1))

/* turn on right-most 0-bit in x */
#define BITOP_TURNON_0(x)    ((x) |= ((x)+1))

inline void Int8ToBinary(CxxAbb::Int8 _num, std::ostream & _os)
{
	char str[9] = {0};
	int i;
	for (i=7; i>=0; i--)
	{
		str[i] = (_num&1)?'1':'0';
		_num >>= 1;
	}
	_os << str;
}

inline void Int16ToBinary(CxxAbb::Int16 _num, std::ostream & _os)
{
	char str[17] = {0};
	int i;
	for (i=15; i>=0; i--)
	{
		str[i] = (_num&1)?'1':'0';
		_num >>= 1;
	}
	_os << str;
}

inline void Int32ToBinary(CxxAbb::Int32 _num, std::ostream & _os)
{
	char str[33] = {0};
	int i;
	for (i=31; i>=0; i--)
	{
		str[i] = (_num&1)?'1':'0';
		_num >>= 1;
	}
	_os << str;
}

inline void Int64ToBinary(CxxAbb::Int64 _num, std::ostream & _os)
{
	char str[65] = {0};
	int i;
	for (i=63; i>=0; i--)
	{
		str[i] = (_num&1)?'1':'0';
		_num >>= 1;
	}
	_os << str;
}

} /* namespace CxxAbb */

#endif /* CXXABB_CORE_BITOPS_H_ */

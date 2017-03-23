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
 * Nullable.h
 *
 * FileId      : $Id: Nullable.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Null wrapper for data types
 *
 */

#ifndef CXXABB_CORE_NULLABLE_H_
#define CXXABB_CORE_NULLABLE_H_


#include <CxxAbb/Core.h>

namespace CxxAbb
{

/** @brief Null wrapper for data types which do not provide null semantic (primitive data types)
 */
template <typename T>
class CXXABB_API Nullable
{
public:
	Nullable() : m_Data(), b_Null(true)
	{}

	Nullable(const T & _t) : m_Data(_t), b_Null(false)
	{}

	Nullable(const Nullable & _rhs) : m_Data(_rhs.m_Data), b_Null(_rhs.b_Null)
	{}

	~Nullable()
	{}

	Nullable& operator = (const T& _t)
	{
		m_Data = _t;
		b_Null = false;
		return *this;
	}

	Nullable& operator = (const Nullable& _rhs)
	{
		m_Data = _rhs.m_Data;
		b_Null = _rhs.b_Null;
		return *this;
	}

	void Swap(Nullable& other)
	{
		std::swap(m_Data, other.m_Data);
		std::swap(b_Null, other.b_Null);
	}

	const T& Value() const
	{
		if (!b_Null)
			return m_Data;
		else
			throw CxxAbb::NullValueException();
	}

	const T& Value(const T& _def) const
	{
		return b_Null ? _def : m_Data;
	}

	bool IsNull() const
	{
		return b_Null;
	}

	void Clear()
	{
		b_Null = true;
	}

private:
	T m_Data;
	bool b_Null;
};

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_NULLABLE_H_ */

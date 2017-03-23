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
 * TypeInfo.h
 *
 * FileId      : $Id: TypeInfo.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Jul 16, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Type information wrapper
 *
 */

#ifndef CXXABB_CORE_TYPEINFO_H_
#define CXXABB_CORE_TYPEINFO_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include <typeinfo>

namespace CxxAbb
{

/** @brief Type information class
 *
 * wrapping std::type_info
 * Uses RTTI
 *
 */
template <typename T>
class CXXABB_API TypeInfo : private NonCopyable
{
public:
	~TypeInfo()
	{}

	TypeInfo(T& _type)
	{
		m_TypInfo = &typeid(_type);
	}

	TypeInfo(const std::type_info& _rTi)
		: m_TypInfo(&_rTi)
	{}

    const std::type_info& Get() const
    { return *m_TypInfo; }

    bool IsEqual(const TypeInfo& _rhs)
    {
    	return *m_TypInfo == _rhs.Get();
    }

    template <typename Other>
    bool IsEqual(const Other & _rhs)
	{
    	return *m_TypInfo == typeid(_rhs);
	}

    bool operator == (const TypeInfo& _rhs)
	{
    	return *m_TypInfo == _rhs.Get();
	}

    const char * Name() const
    {
    	return m_TypInfo->name();
    }

private:
	TypeInfo() : m_TypInfo(0)
	{}


	const std::type_info * m_TypInfo;
};

} /* namespace CxxAbb */

#endif /* CXXABB_CORE_TYPEINFO_H_ */

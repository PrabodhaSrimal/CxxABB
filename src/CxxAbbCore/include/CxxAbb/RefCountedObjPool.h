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
 * RefCountedObjPool.h
 *
 * FileId      : $Id: RefCountedObjPool.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Pool manager for ref-counted objects
 *
 */

#ifndef CXXABB_CORE_REFCOUNTEDOBJPOOL_H_
#define CXXABB_CORE_REFCOUNTEDOBJPOOL_H_

#include <CxxAbb/Core.h>
#include <list>

namespace CxxAbb
{

/** @brief Garbage collection pool for ref-counted objects
 *
 * Ref-counted object should follow CxxAbb::RefCountedObj interface (or derive)
 * Pooled objects will be released by manually calling Release() or
 * later at the pool destruction.
 *
 */
template <class Obj>
class RefCountedObjPool
{
	typedef Obj* StoredType;
public:

	RefCountedObjPool()
	{}

	~RefCountedObjPool()
	{
		Release();
	}

	/// For AutoPtr use duplicate() to get a raw pointer
	void Add(StoredType _pObj)
	{
		lst_ObjectList.push_back(_pObj);
	}

	/// Costly operation, assumed occasionally called
	bool Remove(StoredType _pObj)
	{
		Iterator iteBegin = lst_ObjectList.begin();
		Iterator iteEnd = lst_ObjectList.end();

		while(iteBegin != iteEnd)
		{
			if((*iteBegin) == _pObj)
			{
				lst_ObjectList.erase(iteBegin);
				return true;
			}

			++iteBegin;
		}

		return false;
	}

	void Release()
	{
		Iterator iteBegin = lst_ObjectList.begin();
		Iterator iteEnd = lst_ObjectList.end();

		while(iteBegin != iteEnd)
		{
			(*iteBegin)->refRem();

			++iteBegin;
		}

		lst_ObjectList.clear();
	}

private:
	typedef typename std::list<StoredType> ObjectList;
	typedef typename std::list<StoredType>::iterator Iterator;

	ObjectList lst_ObjectList;
};

} /* namespace CxxAbb */



#endif /* CXXABB_CORE_REFCOUNTEDOBJPOOL_H_ */

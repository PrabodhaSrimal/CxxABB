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
 * Singleton.h
 *
 * FileId      : $Id: Singleton.h 26 2017-02-07 18:30:33Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Feb 22, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2017-02-08 00:00:33 +0530 (Wed, 08 Feb 2017) $
 * Version     : $Revision: 26 $
 *
 * Library     : CxxAbbCore
 * Module      : Pattern
 * Comment     : Singleton design pattern
 *
 */

#ifndef CXXABB_DP_SINGLETON_H_
#define CXXABB_DP_SINGLETON_H_

#include "CxxAbb/NonCopyable.h"


namespace CxxAbb
{
namespace Dp
{

/** @brief Singleton design pattern helper
 *
 * Use:
 * @code
 *  class SingletonClass : public Singleton<SingletonClass>
 *  {
 *      friend class Singleton<SingletonClass>;
 *      ...
 *    private:
 *      SingletonClass() {}
 *      ...
 *    public:
 *      ...
 *  }
 * @endcode
 */
template <class SC>
class Singleton : public NonCopyable
{
public:

	static SC* Instance()
	{
		//TODO: use a lock here
		if(!p_Instance)
		{
			p_Instance = new SC;
		}
		return p_Instance;
	}

//	static SC & Instance()
//	{
//		static SC mInstance;
//		return mInstance;
//	}

protected:
	Singleton()
	{}

	virtual ~Singleton()
	{
		if(p_Instance)
			delete p_Instance;
	}

private:
	static SC* p_Instance;
};

template <class SC>
SC* Singleton<SC>::p_Instance = NULL;

} /* namespace Dp */
} /* namespace CxxAbb */


#endif /* CXXABB_DP_SINGLETON_H_ */

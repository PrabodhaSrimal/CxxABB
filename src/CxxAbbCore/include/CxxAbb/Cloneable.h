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
 * Cloneable.h
 *
 * FileId      : $Id: Cloneable.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Aug 26, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Policy
 * Comment     : Deep copy policy
 *
 */

#ifndef CXXABB_CORE_CLONEABLE_H_
#define CXXABB_CORE_CLONEABLE_H_

#include <CxxAbb/Core.h>

namespace CxxAbb
{

/// Still have no Idea how to use this - should improve with templates

class CXXABB_API Cloneable
{
public:
	Cloneable()
	{}

	virtual ~Cloneable() // Not virtual
	{}

	virtual Cloneable& Clone() const = 0;
};

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_CLONEABLE_H_ */

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
 * RefCountedObj.h
 *
 * FileId      : $Id: RefCountedObj.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 4, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Base class for refcounted objects. To be use with Smart pointers.
 *
 */

#ifndef CXXABB_REFCOUNTEDOBJ_H_
#define CXXABB_REFCOUNTEDOBJ_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include <CxxAbb/Sys/Atomicity.h>

namespace CxxAbb
{

/** @brief Reference counting facility for objects, To be use with Smart Pointers
 *
 * Whenever a reference is destroyed or overwritten, the reference
 * count of the object it references is decremented.
 * Whenever a reference is created or copied, the reference count of
 * the object it references is incremented.
 * The initial reference count of an object is one.
 * When the reference count of an object reaches zero, the object is
 * deleted.
 * In a multithreaded scenario, incrementing and decrementing/
 * comparing reference counters must be atomic operations.
 *
 */
class RefCounted : private NonCopyable
{
public:
	RefCounted()
	: ui_RefCount(1)
	{ }

	explicit RefCounted(unsigned int _uiRefs)
	: ui_RefCount(_uiRefs)
	{ }

	virtual ~RefCounted()
	{ }

	virtual unsigned int refAdd()
	{ return ++ui_RefCount; }

	virtual void refRem()
	{
		if(--ui_RefCount == 0)
			delete this;
	}

	unsigned int refCount() const
	{ return ui_RefCount; }

private:
	unsigned int ui_RefCount;
};

/** @brief Reference counting facility with atomic-counter
 *
 * Whenever a reference is destroyed or overwritten, the reference
 * count of the object it references is decremented.
 * Whenever a reference is created or copied, the reference count of
 * the object it references is incremented.
 * The initial reference count of an object is one.
 * When the reference count of an object reaches zero, the object is
 * deleted.
 * In a multithreaded scenario, incrementing and decrementing/
 * comparing reference counters must be atomic operations.
 *
 */
class AtomicRefCounted : private NonCopyable
{
public:
	AtomicRefCounted()
	: m_AtomicCounter()
	{ }

	explicit AtomicRefCounted(unsigned int _uiRefs)
	: m_AtomicCounter(_uiRefs)
	{ }

	virtual ~AtomicRefCounted()
	{ }

	virtual int refAdd()
	{
		++m_AtomicCounter;
		return (int)m_AtomicCounter;
	}

	virtual void refRem()
	{
		if (!(--m_AtomicCounter))
			delete this;
	}

	int refCount() const
	{ return (int)m_AtomicCounter; }

private:
	mutable CxxAbb::Sys::AtomicCounter m_AtomicCounter;
};

} /* namespace CxxAbb */


#endif /* CXXABB_REFCOUNTEDOBJ_H_ */

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
 * SmartPtr.h
 *
 * FileId      : $Id: SmartPtr.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 3, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Smart Pointers
 *
 */

#ifndef CXXABB_CORE_SMARTPTR_H_
#define CXXABB_CORE_SMARTPTR_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Debug.h>
#include <CxxAbb/Exception.h>

#include <memory>

namespace CxxAbb
{

/** @brief Destroy policy for objects allocated by "new"
 * The NULL pointer is also valid
 */
template<class Obj>
class DeleteDestroyPolicy
{
public:
	static void destroy(Obj* &_p)
	{
		delete _p;
		_p = NullPtr;
	}
};

/** @brief Destroy policy for objects allocated by "new[]"
 * The NULL pointer is also valid
 */
template<class Obj>
class ArrayDeleteDestroyPolicy
{
public:
	static void destroy(Obj* &_p)
	{
		delete [] _p;
		_p = NullPtr;
	}
};

/** @brief Destroy policy for objects allocated by "malloc"
 * The NULL pointer is not-valid
 */
template<class Obj>
class FreeDestroyPolicy
{
public:
	static void destroy(Obj* &_p)
	{
		free(_p);
		_p = NullPtr;
	}
};

/** @brief Intrusive reference counting policy for smart pointers
 */
template<class Obj, class DestroyPolicy>
class ExternalRefCounter
{
public:
	ExternalRefCounter()
		: ui_RefCount(1)
	{}

	void link()
	{
		++ui_RefCount;
	}

	bool unlink(Obj * &_ptr)
	{
		if (--ui_RefCount == 0)
		{
			DestroyPolicy::destroy(_ptr);
			return true;
		}
		return false;
	}

	unsigned int count() const
	{
		return ui_RefCount;
	}


private:
	ExternalRefCounter(const ExternalRefCounter &);
	ExternalRefCounter& operator = (ExternalRefCounter &);

	unsigned int ui_RefCount;
};

template <class Obj>
class ExternalRefCountedDeleter
{
public:
	ExternalRefCountedDeleter()
		: ui_RefCount(1)
	{}

	void link()
	{
		++ui_RefCount;
	}

	bool unlink(Obj * &_ptr)
	{
		if (--ui_RefCount == 0)
		{
			delete _ptr;
			_ptr = NullPtr;
			return true;
		}
		return false;
	}

	unsigned int count() const
	{
		return ui_RefCount;
	}

private:
	ExternalRefCountedDeleter(const ExternalRefCountedDeleter &);
	ExternalRefCountedDeleter& operator = (ExternalRefCountedDeleter &);

	unsigned int ui_RefCount;
};

template <class Obj>
class ExternalRefCountedArrayDeleter
{
public:
	ExternalRefCountedArrayDeleter()
		: ui_RefCount(1)
	{}

	void link()
	{
		++ui_RefCount;
	}

	bool unlink(Obj * &_ptr)
	{
		if (--ui_RefCount == 0)
		{
			delete [] _ptr;
			_ptr = NullPtr;
			return true;
		}
		return false;
	}

	unsigned int count() const
	{
		return ui_RefCount;
	}

private:
	ExternalRefCountedArrayDeleter(const ExternalRefCountedArrayDeleter &);
	ExternalRefCountedArrayDeleter& operator = (ExternalRefCountedArrayDeleter &);

	unsigned int ui_RefCount;
};


/** @brief Internal Reference count based garbage collection for raw pointers
 *
 * External reference counted - Pointee object must maintain reference
 * count. And must implement following public methods
 *
 * @code
 * unsigned int refAdd() // Increment refcount by one
 * void refRem() // Decrement refcount by one and at zero delete object
 * @endcode
 *
 * At the constructor of the pointed object, reference count set to one.
 *
 * AutoPtr takes the ownership of object set to it by calling ctor or
 * assign() or assignment operator.
 * Once raw pointer is assigned and managed by AutoPtr, beware to avoid
 * the use of raw pointer.
 *
 */
template <class Obj>
class AutoPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;

public:
	AutoPtr() : p_Obj(NullPtr)
	{

	}

	AutoPtr(Obj* _obj)  : p_Obj(_obj)
	{
	}

	AutoPtr(Obj* _obj, bool _shared)  : p_Obj(_obj)
	{
		if (_shared && p_Obj) p_Obj->refAdd();
	}

	AutoPtr(const AutoPtr& _ptr)  : p_Obj(_ptr.p_Obj)
	{
		if (p_Obj) p_Obj->refAdd();
	}

	template <class Other>
	explicit AutoPtr(const AutoPtr<Other>& _ptr)  : p_Obj(const_cast<Other*>(_ptr.get()))
	{
		if (p_Obj) p_Obj->refAdd();
	}

	~AutoPtr()
	{
		if (p_Obj) p_Obj->refRem();
	}

	/// Assign new raw pointer and take sole ownership
	/// Current Object pointed by this will be released
	AutoPtr& assign(Obj* _obj)
	{
		if (p_Obj != _obj)
		{
			if (p_Obj) p_Obj->refRem();
			p_Obj = _obj;
		}
		return *this;
	}

	/// Assign new raw pointer and take shared ownership
	/// Current Object pointed by this will be released
	AutoPtr& assign(Obj* _obj, bool _shared)
	{
		if (p_Obj != _obj)
		{
			if (p_Obj) p_Obj->refRem();
			p_Obj = _obj;
			if (_shared && p_Obj) p_Obj->refAdd();
		}
		return *this;
	}

	/// Assign new raw pointer and take shared ownership
	/// Current Object pointed by this will be released
	AutoPtr& assign(const AutoPtr& _ptr)
	{
		if (&_ptr != this)
		{
			if (p_Obj) p_Obj->refRem();
			p_Obj = _ptr.p_Obj;
			if (p_Obj) p_Obj->refAdd();
		}
		return *this;
	}

	template <class Other>
	AutoPtr& assign(const AutoPtr<Other>& ptr)
	{
		if (ptr.get() != p_Obj)
		{
			if (p_Obj) p_Obj->refRem();
			p_Obj = const_cast<Other*>(ptr.get());
			if (p_Obj) p_Obj->refAdd();
		}
		return *this;
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	/// Get a copy of pointed object safely
	Obj* duplicate()
	{
		if (p_Obj) p_Obj->refAdd();
		return p_Obj;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		if(p_Obj != _pObj)
		{
			if(p_Obj) p_Obj->refRem();
			p_Obj = _pObj;
		}
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return (p_Obj == NullPtr);
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	AutoPtr& operator = (Obj* _obj)
	{
		return assign(_obj);
	}

	AutoPtr& operator = (const AutoPtr& _ptr)
	{
		return assign(_ptr);
	}

	template <class Other>
	AutoPtr& operator = (const AutoPtr<Other>& _ptr)
	{
		return assign<Other>(_ptr);
	}

	bool operator == (const AutoPtr& _ptr) const
	{
		return p_Obj == _ptr.p_Obj;
	}

	bool operator == (const Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator == (Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator != (const AutoPtr& _ptr) const
	{
		return p_Obj != _ptr.p_Obj;
	}

	bool operator != (const Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator != (Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator < (const AutoPtr& _ptr) const
	{
		return p_Obj < _ptr.p_Obj;
	}

	bool operator < (const Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator < (Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator <= (const AutoPtr& _ptr) const
	{
		return p_Obj <= _ptr.p_Obj;
	}

	bool operator <= (const Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator <= (Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator > (const AutoPtr& _ptr) const
	{
		return p_Obj > _ptr.p_Obj;
	}

	bool operator > (const Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator > (Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator >= (const AutoPtr& _ptr) const
	{
		return p_Obj >= _ptr.p_Obj;
	}

	bool operator >= (const Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	bool operator >= (Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	void swap(AutoPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
	}

	/// Cast via dynamic_cast between class hierarchy
	template <class Other>
	AutoPtr<Other> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(p_Obj);
		return AutoPtr<Other>(pOther, true);
	}

	/// Cast via static_cast between class hierarchy
	template <class Other>
	AutoPtr<Other> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(p_Obj);
		return AutoPtr<Other>(pOther, true);
	}

private:
	StoredType p_Obj;
};

/** @brief Smart pointer with life time limited to defined scope
 * Strict Ownership - Should be only one owner for pointee
 * NonCopyable - Pointer cannot assign or copy (no assignment op).
 * Lifetime Scoped - Pointee is destroyed once getting out of scope.
 */
template<class Obj>
class ScopedPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;
	typedef ScopedPtr<Obj> ThisType;
public:
	explicit ScopedPtr(Obj* _obj = NullPtr) : p_Obj(_obj)
	{
	}

	explicit ScopedPtr(std::auto_ptr<Obj>& _ptr) : p_Obj( _ptr.release() )
    {
    }

	~ScopedPtr()
	{
		delete p_Obj; // delete on NULL has no effect
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	/// Release ownership
	Obj* release()
	{
		Obj * pRet(p_Obj);
		p_Obj = NullPtr;
		return pRet;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		ASSERT((_pObj == NullPtr) || (p_Obj != _pObj));
		ThisType(_pObj).swap(*this); // Guaranteed the pointee deletion
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return p_Obj == NullPtr;
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	void swap(ScopedPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
	}

private:
	// private copy-ctor and assignment operators
	ScopedPtr(const ScopedPtr& _ptr);

	ScopedPtr& operator = (Obj* _obj);
	ScopedPtr& operator = (const ScopedPtr& _ptr);
	template <class Other>
	ScopedPtr& operator = (const AutoPtr<Other>& _ptr);

	bool operator == (const ScopedPtr& _ptr) const;
	bool operator == (const Obj* _ptr) const;
	bool operator == (Obj* _ptr) const;
	bool operator != (const ScopedPtr& _ptr) const;
	bool operator != (const Obj* _ptr) const;
	bool operator != (Obj* _ptr) const;


	StoredType p_Obj;
};

/** @brief Smart pointer for arrays with life time limited to defined scope
 * Strict Ownership - Should be only one owner for pointee
 * NonCopyable - Pointer cannot assign or copy (no assignment op).
 * Lifetime Scoped - Pointee is destroyed once getting out of scope.
 */
template<class Obj>
class ScopedArrayPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;
	typedef ScopedArrayPtr<Obj> ThisType;
public:
	explicit ScopedArrayPtr(Obj* _obj = NullPtr) : p_Obj(_obj)
	{
	}

	explicit ScopedArrayPtr(std::auto_ptr<Obj>& _ptr) : p_Obj( _ptr.release() )
    {
    }

	~ScopedArrayPtr()
	{
		delete [] p_Obj; // delete on NULL has no effect
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	/// Release ownership
	Obj* release()
	{
		Obj * pRet(p_Obj);
		p_Obj = NullPtr;
		return pRet;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		ASSERT((_pObj == NullPtr) || (p_Obj != _pObj));
		ThisType(_pObj).swap(*this); // Guaranteed the pointee deletion
	}

	Obj& operator [] (std::ptrdiff_t _idx) const
	{
		ASSERT(_idx > 0);
		ASSERT(p_Obj != NullPtr);
		return p_Obj[_idx];
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return p_Obj == NullPtr;
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	void swap(ScopedArrayPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
	}

private:
	// private copy-ctor and assignment operators
	ScopedArrayPtr(const ScopedArrayPtr& _ptr);

	ScopedArrayPtr& operator = (Obj* _obj);
	ScopedArrayPtr& operator = (const ScopedArrayPtr& _ptr);

	bool operator == (const ScopedArrayPtr& _ptr) const;
	bool operator == (const Obj* _ptr) const;
	bool operator == (Obj* _ptr) const;
	bool operator != (const ScopedArrayPtr& _ptr) const;
	bool operator != (const Obj* _ptr) const;
	bool operator != (Obj* _ptr) const;


	StoredType p_Obj;
};

/** @brief External reference count based Smart Pointer for "new" allocated pointers
 *
 */
template<class Obj>
class SharedPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;
	typedef SharedPtr<Obj> ThisType;
	typedef ExternalRefCountedDeleter<Obj> OwnershipPolicy;

public:
	SharedPtr() : p_Obj(NullPtr), p_Counter(new OwnershipPolicy)
	{
	}

	SharedPtr(Obj* _obj)  : p_Obj(_obj), p_Counter(new OwnershipPolicy)
	{
	}

	SharedPtr(const SharedPtr& _ptr)  : p_Obj(_ptr.p_Obj), p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}

	template <class Other>
	explicit SharedPtr(const SharedPtr<Other>& _ptr)
		: p_Obj(const_cast<Other*>(_ptr.get())),
		  p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}

	~SharedPtr()
	{
		Release();
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	unsigned int refCount() const
	{
		return p_Counter->count();
	}

	bool isUnique() const
	{
		return p_Counter->count() == 1;
	}

	/// Assign new raw pointer and take sole ownership
	/// Current Object pointed by this will be released
	SharedPtr& assign(Obj* _obj)
	{
		if (p_Obj != _obj)
		{
			Release();
			p_Counter = new OwnershipPolicy();
			p_Obj = _obj;
		}
		return *this;
	}

	/// Assign new Smart pointer and take shared ownership
	/// Current Object pointed by this will be released
	SharedPtr& assign(const SharedPtr& _ptr)
	{
		if (&_ptr != this)
		{
			SharedPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other>
	SharedPtr& assign(const SharedPtr<Other>& _ptr)
	{
		if (_ptr.get() != p_Obj)
		{
			SharedPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		ThisType().swap(*this);
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return p_Obj == NullPtr;
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	SharedPtr& operator = (Obj* _obj)
	{
		return assign(_obj);
	}

	SharedPtr& operator = (const SharedPtr& _ptr)
	{
		return assign(_ptr);
	}

	template <class Other>
	SharedPtr& operator = (const SharedPtr<Other>& _ptr)
	{
		return assign<Other>(_ptr);
	}

	bool operator == (const SharedPtr& _ptr) const
	{
		return p_Obj == _ptr.p_Obj;
	}

	bool operator == (const Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator == (Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator != (const SharedPtr& _ptr) const
	{
		return p_Obj != _ptr.p_Obj;
	}

	bool operator != (const Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator != (Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator < (const SharedPtr& _ptr) const
	{
		return p_Obj < _ptr.p_Obj;
	}

	bool operator < (const Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator < (Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator <= (const SharedPtr& _ptr) const
	{
		return p_Obj <= _ptr.p_Obj;
	}

	bool operator <= (const Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator <= (Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator > (const SharedPtr& _ptr) const
	{
		return p_Obj > _ptr.p_Obj;
	}

	bool operator > (const Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator > (Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator >= (const SharedPtr& _ptr) const
	{
		return p_Obj >= _ptr.p_Obj;
	}

	bool operator >= (const Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	bool operator >= (Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	void swap(SharedPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
		std::swap(p_Counter, _ptr.p_Counter);
	}

	/// Cast via dynamic_cast between class hierarchy
	template <class Other>
	SharedPtr<Other> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(p_Obj);
		if(pOther)
			return SharedPtr<Other>(p_Counter, pOther);
		return SharedPtr<Other>();
	}

	/// Cast via static_cast between class hierarchy
	template <class Other>
	SharedPtr<Other> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(p_Obj);
		return SharedPtr<Other>(p_Counter, pOther);
	}


private:
	void Release()
	{
		ASSERT(p_Counter != NullPtr);

		if(p_Counter->unlink(p_Obj))
		{
			delete p_Counter;
			p_Counter = NullPtr;
		}
	}

	SharedPtr(OwnershipPolicy* _pCounter, Obj* _ptr) : p_Counter(_pCounter), p_Obj(_ptr)
	{
		ASSERT (p_Counter);
		p_Counter->link();
	}

	StoredType p_Obj;
	OwnershipPolicy * p_Counter;
};

/** @brief External reference count based Smart Pointer for new[] allocated pointers
 *
 */
template<class Obj>
class SharedArrayPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;
	typedef SharedArrayPtr<Obj> ThisType;
	typedef ExternalRefCountedArrayDeleter<Obj> OwnershipPolicy;

public:
	SharedArrayPtr() : p_Obj(NullPtr), p_Counter(new OwnershipPolicy)
	{
	}

	SharedArrayPtr(Obj* _obj)  : p_Obj(_obj), p_Counter(new OwnershipPolicy)
	{
	}

	SharedArrayPtr(const SharedArrayPtr& _ptr)  : p_Obj(_ptr.p_Obj), p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}
	template <class Other>
	explicit SharedArrayPtr(const SharedArrayPtr<Other>& _ptr)
		: p_Obj(const_cast<Other*>(_ptr.get())),
		  p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}

	~SharedArrayPtr()
	{
		Release();
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	unsigned int refCount() const
	{
		return p_Counter->count();
	}

	bool isUnique() const
	{
		return p_Counter->count() == 1;
	}

	/// Assign new raw pointer and take sole ownership
	/// Current Object pointed by this will be released
	SharedArrayPtr& assign(Obj* _obj)
	{
		if (p_Obj != _obj)
		{
			Release();
			p_Counter = new OwnershipPolicy();
			p_Obj = _obj;
		}
		return *this;
	}

	/// Assign new Smart pointer and take shared ownership
	/// Current Object pointed by this will be released
	SharedArrayPtr& assign(const SharedArrayPtr& _ptr)
	{
		if (&_ptr != this)
		{
			SharedArrayPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other>
	SharedArrayPtr& assign(const SharedArrayPtr<Other>& _ptr)
	{
		if (_ptr.get() != p_Obj)
		{
			SharedArrayPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		ThisType().swap(*this);
	}

	Obj& operator [] (std::ptrdiff_t _idx) const
	{
		ASSERT(_idx > 0);
		ASSERT(p_Obj != NullPtr);
		return p_Obj[_idx];
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return p_Obj == NullPtr;
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	SharedArrayPtr& operator = (Obj* _obj)
	{
		return assign(_obj);
	}

	SharedArrayPtr& operator = (const SharedArrayPtr& _ptr)
	{
		return assign(_ptr);
	}

	template <class Other>
	SharedArrayPtr& operator = (const SharedArrayPtr<Other>& _ptr)
	{
		return assign<Other>(_ptr);
	}

	bool operator == (const SharedArrayPtr& _ptr) const
	{
		return p_Obj == _ptr.p_Obj;
	}

	bool operator == (const Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator == (Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator != (const SharedArrayPtr& _ptr) const
	{
		return p_Obj != _ptr.p_Obj;
	}

	bool operator != (const Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator != (Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator < (const SharedArrayPtr& _ptr) const
	{
		return p_Obj < _ptr.p_Obj;
	}

	bool operator < (const Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator < (Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator <= (const SharedArrayPtr& _ptr) const
	{
		return p_Obj <= _ptr.p_Obj;
	}

	bool operator <= (const Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator <= (Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator > (const SharedArrayPtr& _ptr) const
	{
		return p_Obj > _ptr.p_Obj;
	}

	bool operator > (const Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator > (Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator >= (const SharedArrayPtr& _ptr) const
	{
		return p_Obj >= _ptr.p_Obj;
	}

	bool operator >= (const Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	bool operator >= (Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	void swap(SharedArrayPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
		std::swap(p_Counter, _ptr.p_Counter);
	}

	/// Cast via dynamic_cast between class hierarchy
	template <class Other>
	SharedArrayPtr<Other> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(p_Obj);
		if(pOther)
			return SharedArrayPtr<Other>(p_Counter, pOther);
		return SharedArrayPtr<Other>();
	}

	/// Cast via static_cast between class hierarchy
	template <class Other>
	SharedArrayPtr<Other> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(p_Obj);
		return SharedArrayPtr<Other>(p_Counter, pOther);
	}


private:
	void Release()
	{
		ASSERT(p_Counter != NullPtr);

		if(p_Counter->unlink(p_Obj))
		{
			delete p_Counter;
			p_Counter = NullPtr;
		}
	}

	SharedArrayPtr(OwnershipPolicy* _pCounter, Obj* _ptr) : p_Counter(_pCounter), p_Obj(_ptr)
	{
		ASSERT (p_Counter);
		p_Counter->link();
	}

	StoredType p_Obj;
	OwnershipPolicy * p_Counter;
};

/// Policy based smart pointers

template<class Obj,
	class ReleasePolicy = DeleteDestroyPolicy<Obj>,
	class OwnershipPolicy = ExternalRefCounter<Obj, ReleasePolicy> >
class SmartPtr
{
	typedef Obj* StoredType;
	typedef Obj* PointerType;
	typedef Obj& RefrenceType;
	typedef OwnershipPolicy* RefCountType;
	typedef SmartPtr<Obj, ReleasePolicy, OwnershipPolicy> ThisType;

public:
	SmartPtr()  : p_Obj(NullPtr), p_Counter(new OwnershipPolicy)
	{
	}

	SmartPtr(Obj* _obj)  : p_Obj(_obj), p_Counter(new OwnershipPolicy)
	{
	}

	SmartPtr(const SmartPtr& _ptr)  : p_Obj(_ptr.p_Obj), p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}

	template <class Other, class OtherReleasePolicy>
	explicit SmartPtr(const SmartPtr<Other, OtherReleasePolicy, OwnershipPolicy>& _ptr)
		: p_Obj(const_cast<Other*>(_ptr.get())),
		  p_Counter(_ptr.p_Counter)
	{
		p_Counter->link();
	}

	~SmartPtr()
	{
		Release();
	}

	bool isNull() const
	{
		return p_Obj == NullPtr;
	}

	unsigned int refCount() const
	{
		return p_Counter->count();
	}

	/// Assign new raw pointer and take sole ownership
	/// Current Object pointed by this will be released
	SmartPtr& assign(Obj* _obj)
	{
		if (p_Obj != _obj)
		{
			Release();
			p_Counter = new OwnershipPolicy();
			p_Obj = _obj;
		}
		return *this;
	}

	/// Assign new Smart pointer and take shared ownership
	/// Current Object pointed by this will be released
	SmartPtr& assign(const SmartPtr& _ptr)
	{
		if (&_ptr != this)
		{
			SmartPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other, class OtherReleasePolicy>
	SmartPtr& assign(const SmartPtr<Other, OtherReleasePolicy, OwnershipPolicy>& _ptr)
	{
		if (_ptr.get() != p_Obj)
		{
			SmartPtr tmp(_ptr);
			swap(tmp);
		}
		return *this;
	}

	/// Reset pointed object
	void reset(Obj* _pObj = NullPtr)
	{
		ThisType().swap(*this);
	}

	Obj* operator -> ()
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	const Obj* operator -> () const
	{
		if (p_Obj)
			return p_Obj;
		else
			throw NullPointerException();
	}

	Obj& operator * ()
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	const Obj& operator * () const
	{
		if (p_Obj)
			return *p_Obj;
		else
			throw NullPointerException();
	}

	Obj* get()
	{
		return p_Obj;
	}

	const Obj* get() const
	{
		return p_Obj;
	}

	bool operator ! () const
	{
		return p_Obj == NullPtr;
	}

	operator bool () const
	{
		return p_Obj == NullPtr;
	}

	SmartPtr& operator = (Obj* _obj)
	{
		return assign(_obj);
	}

	SmartPtr& operator = (const SmartPtr& _ptr)
	{
		return assign(_ptr);
	}

	template <class Other, class OtherReleasePolicy>
	SmartPtr& operator = (const SmartPtr<Other, OtherReleasePolicy, OwnershipPolicy>& _ptr)
	{
		return assign<Other>(_ptr);
	}

	bool operator == (const SmartPtr& _ptr) const
	{
		return p_Obj == _ptr.p_Obj;
	}

	bool operator == (const Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator == (Obj* _ptr) const
	{
		return p_Obj == _ptr;
	}

	bool operator != (const SmartPtr& _ptr) const
	{
		return p_Obj != _ptr.p_Obj;
	}

	bool operator != (const Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator != (Obj* _ptr) const
	{
		return p_Obj != _ptr;
	}

	bool operator < (const SmartPtr& _ptr) const
	{
		return p_Obj < _ptr.p_Obj;
	}

	bool operator < (const Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator < (Obj* _ptr) const
	{
		return p_Obj < _ptr;
	}

	bool operator <= (const SmartPtr& _ptr) const
	{
		return p_Obj <= _ptr.p_Obj;
	}

	bool operator <= (const Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator <= (Obj* _ptr) const
	{
		return p_Obj <= _ptr;
	}

	bool operator > (const SmartPtr& _ptr) const
	{
		return p_Obj > _ptr.p_Obj;
	}

	bool operator > (const Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator > (Obj* _ptr) const
	{
		return p_Obj > _ptr;
	}

	bool operator >= (const SmartPtr& _ptr) const
	{
		return p_Obj >= _ptr.p_Obj;
	}

	bool operator >= (const Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	bool operator >= (Obj* _ptr) const
	{
		return p_Obj >= _ptr;
	}

	void swap(SmartPtr& _ptr)
	{
		std::swap(p_Obj, _ptr.p_Obj);
		std::swap(p_Counter, _ptr.p_Counter);
	}

	/// Cast via dynamic_cast between class hierarchy
	template <class Other>
	SmartPtr<Other, ReleasePolicy, OwnershipPolicy> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(p_Obj);
		if(pOther)
			return SmartPtr<Other, ReleasePolicy, OwnershipPolicy>(p_Counter, pOther);
		return SmartPtr<Other, ReleasePolicy, OwnershipPolicy>();
	}

	/// Cast via static_cast between class hierarchy
	template <class Other>
	SmartPtr<Other, ReleasePolicy, OwnershipPolicy> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(p_Obj);
		return SmartPtr<Other, ReleasePolicy, OwnershipPolicy>(p_Counter, pOther);
	}

private:
	template <class Other, class OtherDP, class OtherOP> friend class SmartPtr;

	void Release()
	{
		ASSERT(p_Counter != NullPtr);

		if(p_Counter->unlink(p_Obj))
		{
			delete p_Counter;
			p_Counter = NullPtr;
		}
	}

	SmartPtr(OwnershipPolicy* _pCounter, Obj* _ptr) : p_Counter(_pCounter), p_Obj(_ptr)
	{
		ASSERT (p_Counter);
		p_Counter->link();
	}

	StoredType p_Obj;
	RefCountType p_Counter;
};

// Global functions

template <class Obj>
inline void swap(AutoPtr<Obj>& _p1, AutoPtr<Obj>& _p2)
{
	_p1.swap(_p2);
}

template <class Obj>
inline void swap(ScopedPtr<Obj>& _p1, ScopedPtr<Obj>& _p2)
{
	_p1.swap(_p2);
}

template <class Obj>
inline void swap(ScopedArrayPtr<Obj>& _p1, ScopedArrayPtr<Obj>& _p2)
{
	_p1.swap(_p2);
}

template <class Obj, class DP, class OP>
inline void swap(SmartPtr<Obj, DP, OP>& p1, SmartPtr<Obj, DP, OP>& p2)
{
	p1.swap(p2);
}

template <class Obj>
inline void swap(SharedPtr<Obj>& p1, SharedPtr<Obj>& p2)
{
	p1.swap(p2);
}

template <class Obj>
inline void swap(SharedArrayPtr<Obj>& p1, SharedArrayPtr<Obj>& p2)
{
	p1.swap(p2);
}

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_SMARTPTR_H_ */

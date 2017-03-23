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
 * Buffer.h
 *
 * FileId      : $Id: Buffer.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 12, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Buffer wrapper for data types
 *
 */

#ifndef CXXABB_CORE_BUFFER_H_
#define CXXABB_CORE_BUFFER_H_

#include <CxxAbb/Core.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Debug.h>

namespace CxxAbb
{

/** @brief Array semantic for data types
 *
 * This data structure can be used wherever a data buffer is needed.
 * Direct access to internal data buffer is provided through begin()/end().
 * Direct memcpy() to internal buffer is allowed, but NOT RECOMEMDED.
 * Caller needs to obey capacity and also needs to adjust used size parameter when memcpy().
 */
template <typename T>
class CXXABB_API Buffer
{
public:
	/** @brief Creates and allocates the Buffer.
	 *
	 */
	Buffer(std::size_t _capacity):
		i_Capacity(_capacity),
		i_Used(0),
		p_Data(new T[_capacity]),
		b_Alloced(true)
	{
		std::memset(p_Data,0,i_Capacity);
	}

	/** @brief Creates the Buffer as a wrapper for external data array.
	 * Caller is responsible for allocating and deallocating of external array.
	 *
	 * @param _pMem	Pointer to external array
	 * @param _length	Length of external array
	 */
	explicit Buffer(T* _pMem, std::size_t _length):
		i_Capacity(_length),
		i_Used(_length),
		p_Data(_pMem),
		b_Alloced(false)
	{
	}

	/** @brief Creates the Buffer with a copy of external data array.
	 * External array data is copied to internal array. And life-time is managed.
	 *
	 * @param _pMem	Pointer to external const array
	 * @param _length	Length of external array
	 */
	explicit Buffer(const T* _pMem, std::size_t _length):
		i_Capacity(_length),
		i_Used(_length),
		p_Data(new T[_length]),
		b_Alloced(true)
	{
		if (i_Used)
			std::memcpy(p_Data, _pMem, i_Used * sizeof(T));
	}

	/** @brief Copy constructor.
	 *
	 */
	Buffer(const Buffer& _other):
		i_Capacity(_other.i_Used),
		i_Used(_other.i_Used),
		p_Data(new T[_other.i_Used]),
		b_Alloced(true)
	{
		if (i_Used)
			std::memcpy(p_Data, _other.p_Data, i_Used * sizeof(T));
	}

	/** @brief Destroys the Buffer only if life-time managed
	 *
	 */
	virtual ~Buffer()
	{
		if (b_Alloced)
			delete [] p_Data;
	}

	/** @brief Assignment operator.
	 *
	 */
	virtual Buffer& operator =(const Buffer& _other)
	{
		if (this != &_other)
		{
			Buffer tmp(_other);
			this->swap(tmp);
		}

		return *this;
	}

	/** @brief Resize internal array to new capacity.
	 * Throws a InvalidAccessException if life-time is not managed.
	 * Dose nothing if new size is less or equal to current capacity
	 *
	 * @param _newCapacity	New size of internal array
	 * @param _preserve	Preserve existing data
	 */
	virtual void resize(std::size_t _newCapacity, bool _preserveContent = true)
	{
		if (!b_Alloced)
			throw CxxAbb::InvalidAccessException("Buffer: Illegal access to unmanaged/external buffer");

		ASSERT(_newCapacity);

		if (_newCapacity > i_Capacity)
		{
			T* ptr = new T[_newCapacity];
			if (_preserveContent)
				std::memcpy(ptr, p_Data, i_Capacity);

			delete [] p_Data;
			p_Data  = ptr;
			i_Capacity = _newCapacity;
		}
	}

	/** @brief Assigns the argument buffer to this buffer.
	 * If necessary, resizes the buffer.
	 */
	virtual void assign(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		if (_sz > i_Capacity)
		{
			resize(_sz, false);
		}
		std::memcpy(p_Data, _buf, _sz);
		i_Used = _sz;
	}

	/** @brief Resizes this buffer and appends the argument buffer.
	 *
	 */
	virtual void append(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		std::size_t oldSize = i_Used;
		resize(i_Used + _sz, true);
		std::memcpy(p_Data + oldSize, _buf, _sz);
		i_Used += _sz;
	}

	/** @brief Resizes this buffer and appends the argument buffer.
	 *
	 */
	void append(const Buffer& _buf)
	{
		append(_buf.begin(), _buf.size());
	}

	/** @brief Swaps the buffer with another one.
	 *
	 */
	virtual void swap(Buffer& _other)
	{
		using std::swap;

		ASSERT(i_Capacity >= _other.i_Capacity);

		swap(p_Data, _other.p_Data);
		swap(i_Capacity, _other.i_Capacity);
		swap(i_Used, _other.i_Used);
	}

	/** @brief Returns the allocated memory size.
	 *
	 */
	std::size_t capacity() const
	{
		return i_Capacity;
	}

	/** @brief Compare operator.
	 *
	 */
	bool operator ==(const Buffer& _other) const
	{
		if (this != &_other)
		{
			if (i_Used == _other.i_Used)
			{
				if (std::memcmp(p_Data, _other.p_Data, i_Used) == 0)
				{
					return true;
				}
			}
			return false;
		}

		return true;
	}

	/** @brief Compare operator.
	 *
	 */
	bool operator !=(const Buffer& _other) const
	{
		return !(*this == _other);
	}

	/** @brief Sets the contents of the buffer to zero.
	 *
	 */
	void clear()
	{
		std::memset(p_Data, 0, i_Used * sizeof(T));
		i_Used = 0;
	}

	/** @brief Returns the used size of the buffer.
	 *
	 */
	std::size_t size() const
	{
		return i_Used;
	}

	/** @brief Set used size when externally handle internal buffer
	 *
	 */
	void size(std::size_t _used)
	{
		i_Used = _used;
	}

	/** @brief Returns a pointer to the beginning of the buffer.
	 *
	 */
	T* begin()
	{
		return p_Data;
	}

	/** @brief Returns a pointer to the beginning of the buffer.
	 *
	 */
	const T* begin() const
	{
		return p_Data;
	}

	/** @brief Returns a pointer to end of the buffer.
	 *
	 */
	T* end()
	{
		return p_Data + i_Used;
	}

	/** @brief Returns a pointer to the end of the buffer.
	 *
	 */
	const T* end() const
	{
		return p_Data + i_Used;
	}

	/** @brief Return true if buffer is empty.
	 *
	 */
	bool empty() const
	{
		return 0 == i_Used;
	}

	/** @brief Return true if buffer reached its max size
	 *
	 */
	virtual bool full() const
	{
		return i_Used == i_Capacity;
	}

	T& operator [] (std::size_t _index)
	{
		ASSERT (_index < i_Used);

		return p_Data[_index];
	}

	const T& operator [] (std::size_t _index) const
	{
		ASSERT (_index < i_Used);

		return p_Data[_index];
	}

	/** @brief Remove data from begin of array and move rest to forward
	 *
	 */
	void remove(std::size_t _length)
	{
		if(_length > 0 && _length <= i_Used)
		{
			i_Used -= _length;
			std::memmove(p_Data, p_Data + _length, i_Used);
		}
	}

protected:
	std::size_t i_Capacity;
	std::size_t i_Used;
	T * p_Data;
	bool b_Alloced;

private:

	Buffer();
};


/** @brief Fixed length buffer
 *
 * Buffer has defined fixed length
 * Internal buffer is not resized at append/assign/swap
 * only data that fit in to free memory will be copied
 * others will be ignored silently
 */
template <typename T>
class CXXABB_API FixedLenBuffer : public Buffer<T>
{
public:
	/** @brief Creates and allocates the Buffer.
	 * Internal data reset to zero
	 *
	 * @param _capacity	Fixed size capacity of buffer
	 */
	FixedLenBuffer(std::size_t _capacity)
		: Buffer<T>(_capacity)
	{}

	/** @brief Creates the Buffer as a wrapper for external data array.
	 * Caller is responsible for allocating and deallocating of external array.
	 *
	 * @param _pMem	Pointer to external array
	 * @param _length	Length of external array
	 */
	explicit FixedLenBuffer(T* _pMem, std::size_t _length)
		: Buffer<T>(_pMem, _length)
	{}

	/** @brief Creates the Buffer with a copy of external data array.
	 * External array data is copied to internal array. And life-time is managed.
	 *
	 * @param _pMem	Pointer to external const array
	 * @param _length	Length of external array
	 */
	explicit FixedLenBuffer(const T* _pMem, std::size_t _length)
		: Buffer<T>(_pMem, _length)
	{}

	/** @brief Copy constructor
	 *
	 */
	FixedLenBuffer(const FixedLenBuffer& _other)
		: Buffer<T>(_other)
	{}

	virtual ~FixedLenBuffer()
	{}

	/** @brief Resize fixed length buffer - no resize
	 *
	 * @param _newCapacity	New size of internal array
	 * @param _preserve	Preserve existing data
	 */
	virtual void resize(std::size_t /*_newCapacity*/, bool 	/*_preserveContent*/ = true)
	{
	}

	/** @brief Assigns the argument buffer to this buffer.
	 *
	 */
	virtual void assign(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		if (_sz > this->i_Capacity)
			_sz = this->i_Capacity;
		std::memcpy(this->p_Data, _buf, _sz);
		this->i_Used = _sz;
	}

	/** @brief Resizes this buffer and appends the argument buffer.
	 *
	 */
	virtual void append(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		std::size_t iFree = this->i_Capacity - this->i_Used;
		if(_sz > iFree)
			_sz = iFree;
		std::memcpy(this->p_Data + this->i_Used, _buf, _sz);
		this->i_Used += _sz;
	}

private:
};

/** @brief Spanning buffer (With defined max)
 *
 * Buffer has defined max length
 * Internal buffer is resized up to max limit.
 */
template <typename T>
class CXXABB_API SpanningBuffer : public Buffer<T>
{
public:
	/** @brief Creates and allocates the Buffer.
	 * Internal data reset to zero
	 *
	 * @param _capacity	Max capacity of buffer
	 */
	SpanningBuffer(std::size_t _initSize, std::size_t _maxSize)
		: Buffer<T>(_initSize),
		  i_MaxSize(_maxSize)
	{
		if(_initSize > _maxSize)
		{
			throw InvalidArgumentException("SpanningBuffer: initSize > maxSize");
		}
	}

	/** @brief Creates the Buffer as a wrapper for external data array.
	 * Caller is responsible for allocating and deallocating of external array.
	 *
	 * @param _pMem	Pointer to external array
	 * @param _length	Length of external array
	 */
	explicit SpanningBuffer(T* _pMem, std::size_t _length)
		: Buffer<T>(_pMem, _length),
		  i_MaxSize(_length)
	{}

	/** @brief Creates the Buffer with a copy of external data array.
	 * External array data is copied to internal array. And life-time is managed.
	 *
	 * @param _pMem	Pointer to external const array
	 * @param _length	Length of external array
	 */
	explicit SpanningBuffer(const T* _pMem, std::size_t _length)
		: Buffer<T>(_pMem, _length),
		  i_MaxSize(_length)
	{}

	/** @brief Copy constructor
	 *
	 */
	SpanningBuffer(const SpanningBuffer& _other)
		: Buffer<T>(_other)
	{
		i_MaxSize = _other.i_MaxSize;
	}

	virtual ~SpanningBuffer()
	{}

	/** @brief Resize buffer up to max limit
	 *
	 * @param _newCapacity	New size of internal array
	 * @param _preserve	Preserve existing data
	 */
	virtual void resize(std::size_t _newCapacity, bool 	_preserveContent = true)
	{
		if (!this->b_Alloced)
		{
			throw CxxAbb::InvalidAccessException("Buffer: Illegal access to unmanaged/external buffer");
		}

		ASSERT(_newCapacity);

		if(_newCapacity < this->i_Capacity)
			return;

		if (_newCapacity <= i_MaxSize)
		{
			T* ptr = new T[_newCapacity];
			if (_preserveContent)
				std::memcpy(ptr, this->p_Data, this->i_Capacity);

			delete [] this->p_Data;
			this->p_Data  = ptr;
			this->i_Capacity = _newCapacity;
		}
		else
		{
			throw CxxAbb::OutOfMemoryException("SpanningBuffer: Max limit reached when resize");
		}
	}

	/** @brief Assigns the argument buffer to this buffer.
	 *
	 */
	virtual void assign(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		if (_sz > this->i_Capacity)
			resize(_sz, false);
		std::memcpy(this->p_Data, _buf, _sz);
		this->i_Used = _sz;
	}

	/** @brief Resizes this buffer and appends the argument buffer.
	 *
	 */
	virtual void append(const T* _buf, std::size_t _sz)
	{
		if (0 == _sz) return;
		std::size_t iFree = this->i_Capacity - this->i_Used;

		if(_sz > iFree)
			resize(_sz + this->i_Used);

		std::memcpy(this->p_Data + this->i_Used, _buf, _sz);
		this->i_Used += _sz;
	}

	std::size_t max() const
	{
		return i_MaxSize;
	}

	virtual bool full() const
	{
		return this->i_Used == i_MaxSize;
	}

private:
	std::size_t i_MaxSize;
};

typedef Buffer<char> CharBuffer;
typedef Buffer<CxxAbb::Byte> ByteBuffer;
typedef FixedLenBuffer<CxxAbb::Byte> FixedLenByteBuffer;
typedef SpanningBuffer<CxxAbb::Byte> SpanningByteBuffer;

} /* namespace CxxAbb */


#endif /* CXXABB_CORE_BUFFER_H_ */

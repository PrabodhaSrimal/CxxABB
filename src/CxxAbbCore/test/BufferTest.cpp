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
 * BufferTest.cpp
 *
 * FileId      : $Id: BufferTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 13, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */

#include <CxxAbb/Buffer.h>
#include <gtest/gtest.h>



TEST(BufferTest, Basic)
{
	std::size_t s = 10;

	CxxAbb::Buffer<int> b(s);

	//COUT_LOG() << b.size();
	ASSERT_TRUE (b.size() == 0);
	ASSERT_TRUE (b.capacity() == s);

	std::vector<int> v;
	for (std::size_t i = 0; i < s; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());
	b.size(s);

	ASSERT_TRUE (s == b.size());
}

TEST(BufferTest, FixedLenBuffer)
{
	std::size_t t = 8;

	CxxAbb::FixedLenBuffer<char> b(t);

	ASSERT_TRUE(b.empty());

	//COUT_LOG() << b.size();
	ASSERT_TRUE(0 == b.size());
	ASSERT_TRUE(t == b.capacity());

	const char * z = "hello";
	std::size_t i = strlen(z) + 1;
	b.assign(z, i);

	ASSERT_FALSE(b.empty());
	ASSERT_TRUE(b.size() == i);
	ASSERT_TRUE(b.capacity() == t);

	b.append(z,i);

	ASSERT_TRUE(b.size() == t);
	ASSERT_TRUE(b.capacity() == t);

	b.remove(4);

	ASSERT_TRUE(b.size() == t - 4);

	b.remove(10);

	ASSERT_TRUE(b.size() == t - 4);

	//COUT_LOG() << b.size();
	//COUT_LOG() << b.begin();

	z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	i = strlen(z) + 1;


	CxxAbb::FixedLenBuffer<char> b2(512);
	b2 = b;
//	COUT_LOG() << "B2 : " << b2.size();
	ASSERT_TRUE(b2.size() == (t - 4));

	ASSERT_TRUE(b == b);
	ASSERT_TRUE(b == b2);
	ASSERT_FALSE(b != b2);

	b.assign(z, i);
	ASSERT_TRUE(b.size() == t);
	ASSERT_TRUE(b.capacity() == t);

	ASSERT_FALSE(b == b2);
	ASSERT_TRUE(b != b2);

	b.append(z, i);
	ASSERT_TRUE(b.size() == t);
	ASSERT_TRUE(b.capacity() == t);

	for (std::size_t x=0; x<t; ++x)
	{
		std::size_t j = b.size();
		b.remove(1);
		ASSERT_TRUE(b.size() == (j - 1));
	}

	ASSERT_TRUE(b.empty());

	CxxAbb::FixedLenBuffer<char> d("world",6);
	b.swap(d);
	ASSERT_FALSE(b.empty());

	ASSERT_TRUE(b.size() == 6);
	ASSERT_TRUE(b.capacity() == 6);
	ASSERT_TRUE(d.size() == 0);
	ASSERT_TRUE(d.capacity() == t);

	b.clear();
	ASSERT_TRUE(b.empty());

	ASSERT_THROW((b[5] = 10), CxxAbb::AssertionViolationException);
	//b[5];

	b.assign(z, i);
	//COUT_LOG() << b.begin();
	ASSERT_NO_THROW((b[5] = 'D'));
	//COUT_LOG() << b.begin();

	/// external alloc
	char * ext = new char[20];
	sprintf(ext, "%s", "Hello");
	CxxAbb::FixedLenBuffer<char> eb(ext,20);

	eb.assign("asdadasd",9);


	delete [] ext;
}

TEST(BufferTest, SpanningBuffer)
{
	size_t init = 10;
	size_t max = 100;
	CxxAbb::SpanningBuffer<char> b(init, max);

	ASSERT_TRUE(b.size() == 0);
	ASSERT_TRUE(b.capacity() == init);
	ASSERT_TRUE(b.max() == max);
	ASSERT_TRUE(b.empty());
	ASSERT_FALSE(b.full());

	const char * z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t i = strlen(z);

	b.assign(z, i);
	ASSERT_TRUE(b.size() == i);
	ASSERT_TRUE(b.capacity() == i);
	ASSERT_TRUE(b.max() == max);
	ASSERT_FALSE(b.empty());
	ASSERT_FALSE(b.full());

	b.append(z, i);
	ASSERT_TRUE(b.size() == 2*i);
	ASSERT_TRUE(b.capacity() == 2*i);
	ASSERT_TRUE(b.max() == max);
	ASSERT_FALSE(b.empty());
	ASSERT_FALSE(b.full());

	COUT_LOG() << b.begin();

	b.append(z, i);
	ASSERT_THROW(b.append(z, i), CxxAbb::OutOfMemoryException);

	COUT_LOG() << b.capacity();

	b.clear();
	ASSERT_TRUE(b.empty());

	ASSERT_THROW(CxxAbb::SpanningBuffer<char> b2(max, init), CxxAbb::InvalidArgumentException);
}

namespace
{
	template<class T>
	CxxAbb::Buffer<T> CreateBuffer(int _size)
	{
		CxxAbb::Buffer<T> x(_size);
		return x;
	}
}

//TEST(BufferTest, Ctor)
//{
//	COUT_LOG() << "ctor";
//	CxxAbb::Buffer<int> b(10);
//
//	COUT_LOG() << "copy ctor";
//	CxxAbb::Buffer<int> b2(b);
//
//	COUT_LOG() << "move ctor";
//	CxxAbb::Buffer<int> b3(CreateBuffer<int>(10));
//}

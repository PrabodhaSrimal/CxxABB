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
 * SharedPtrTest.cpp
 *
 * FileId      : $Id: SharedPtrTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 6, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */

#include <CxxAbb/Exception.h>
#include <CxxAbb/SmartPtr.h>
#include <gtest/gtest.h>

namespace {

class TestClass
{
public:
	TestClass(const std::string & _sData)
		: s_Data(_sData)
	{
		i_GlbCount++;
	}

	virtual ~TestClass()
	{
		i_GlbCount--;
	}

	const std::string & Data()
	{
		return s_Data;
	}

	static int count()
	{
		return i_GlbCount;
	}

//	virtual void PolyFunc()
//	{}
private:
	static int i_GlbCount;
	std::string s_Data;
};

int TestClass::i_GlbCount = 0;

class DerivedTestClass : public TestClass
{
public:
	DerivedTestClass(const std::string & _sData, int _i) : TestClass(_sData), i_Num(_i)
	{}

	virtual ~DerivedTestClass()
	{}

	int Number() { return i_Num; }
private:
	int i_Num;
};

}

TEST(SharedPtrTest, Constructor)
{
	CxxAbb::SmartPtr<int> ptrSP1;
	ASSERT_TRUE(ptrSP1.isNull());
	ASSERT_TRUE(ptrSP1.refCount() == 1);
	ASSERT_TRUE(ptrSP1.get() == NULL);

	CxxAbb::SmartPtr<int> ptrSP2(new int);
	ASSERT_FALSE(ptrSP2.isNull());
	ASSERT_TRUE(ptrSP2.refCount() == 1);

	CxxAbb::SmartPtr<int> ptrSP3(ptrSP2);
	ASSERT_FALSE(ptrSP2.isNull());
	ASSERT_FALSE(ptrSP3.isNull());
	ASSERT_TRUE(ptrSP2.refCount() == 2);
	ASSERT_TRUE(ptrSP3.refCount() == 2);

	//CxxAbb::SharedPtr<std::string> ptrStr(new std::string("new string"));

	//CxxAbb::SharedPtr<TestClass> ptrSP4(ptrStr); // <std::string, CxxAbb::DeleteDestroyPolicy>


	CxxAbb::SmartPtr<TestClass> ptr1;
	ASSERT_TRUE(ptr1.get() == NULL);
	TestClass* pTO1 = new TestClass("one");
	TestClass* pTO2 = new TestClass("two");
	if (pTO2 < pTO1)
	{
		TestClass* pTmp = pTO1;
		pTO1 = pTO2;
		pTO2 = pTmp;
	}
	ASSERT_TRUE (pTO1 < pTO2);
	ptr1 = pTO1;
	ASSERT_TRUE (ptr1.refCount() == 1);
	CxxAbb::SmartPtr<TestClass> ptr2 = pTO2;
	CxxAbb::SmartPtr<TestClass> ptr3 = ptr1;
	ASSERT_TRUE (ptr1.refCount() == 2);
	CxxAbb::SmartPtr<TestClass> ptr4;
	ASSERT_TRUE (ptr1.get() == pTO1);
	ASSERT_TRUE (ptr1 == pTO1);
	ASSERT_TRUE (ptr2.get() == pTO2);
	ASSERT_TRUE (ptr2 == pTO2);
	ASSERT_TRUE (ptr3.get() == pTO1);
	ASSERT_TRUE (ptr3 == pTO1);

	ASSERT_TRUE (ptr1 == pTO1);
	ASSERT_TRUE (ptr1 != pTO2);
	ASSERT_TRUE (ptr1 < pTO2);
	ASSERT_TRUE (ptr1 <= pTO2);
	ASSERT_TRUE (ptr2 > pTO1);
	ASSERT_TRUE (ptr2 >= pTO1);

	ASSERT_TRUE (ptr1 == ptr3);
	ASSERT_TRUE (ptr1 != ptr2);
	ASSERT_TRUE (ptr1 < ptr2);
	ASSERT_TRUE (ptr1 <= ptr2);
	ASSERT_TRUE (ptr2 > ptr1);
	ASSERT_TRUE (ptr2 >= ptr1);

	ptr1.swap(ptr2);
	ASSERT_TRUE (ptr2 < ptr1);
	ptr2.swap(ptr1);

	ASSERT_TRUE ((ptr1->Data() == "one" && ptr2->Data() == "two") ||
		(ptr1->Data() == "two" && ptr2->Data() == "one"));

	ASSERT_THROW((ptr4->Data() == "four"), CxxAbb::NullPointerException);

	ASSERT_TRUE (!(ptr4 == ptr1));
	ASSERT_TRUE (!(ptr4 == ptr2));
	ASSERT_TRUE (ptr4 != ptr1);
	ASSERT_TRUE (ptr4 != ptr2);

	ptr4 = ptr2;
	ASSERT_TRUE (ptr4 == ptr2);
	ASSERT_TRUE (!(ptr4 != ptr2));

	ASSERT_TRUE (TestClass::count() == 2);
	ptr1 = 0;
	ptr2 = 0;
	ptr3 = 0;
	ptr4 = 0;
	ASSERT_TRUE (TestClass::count() == 0);

	{
		CxxAbb::SmartPtr<TestClass> ptr = new TestClass("");
		ASSERT_TRUE (TestClass::count() == 1);
	}
	ASSERT_TRUE (TestClass::count() == 0);
}

TEST(SharedPtrTest, Operators)
{
	CxxAbb::SmartPtr<TestClass> ptrSP1;
	ASSERT_TRUE(ptrSP1.isNull());
	ASSERT_TRUE(ptrSP1.refCount() == 1);
	ASSERT_THROW(ptrSP1->count(), CxxAbb::NullPointerException);

//	{
//		// null assign test
//		CxxAbb::SharedPtr<DerivedTestClass> ptr2;
//		ASSERT_TRUE(ptr2.get() == NULL);
//		CxxAbb::SharedPtr<TestClass> ptr1(ptr2);
//	}
//	{
//		CxxAbb::SharedPtr<DerivedTestClass> ptr2(new DerivedTestClass("test", 666));
//		ASSERT_TRUE (TestClass::count() == 1);
//		CxxAbb::SharedPtr<TestClass> ptr1 = ptr2;
//		ASSERT_TRUE (TestClass::count() == 1);
//	}
//	ASSERT_TRUE (TestClass::count() == 0);
//	CxxAbb::SharedPtr<TestClass> ptr1 = new DerivedTestClass("test", 666);
//	ASSERT_TRUE (TestClass::count() == 1);
//	ptr1 = 0;
//	ASSERT_TRUE (TestClass::count() == 0);

}

TEST(SharedPtrTest, Operators2)
{
//	CxxAbb::SmartPtr<TestClass> ptr1 = new DerivedTestClass("test", 666);
//	CxxAbb::SmartPtr<DerivedTestClass> ptr2 = ptr1.cast<DerivedTestClass>();
//	ASSERT_TRUE (ptr2.get() != 0);
//
//	// cast the other way round must fail
//	ptr1 = new TestClass("test");
//	ASSERT_TRUE (TestClass::count() == 2);
//	ptr2 = ptr1.cast<DerivedTestClass>();
//	ASSERT_TRUE (TestClass::count() == 1);
//	ASSERT_TRUE (ptr2.get() == 0);
}
 

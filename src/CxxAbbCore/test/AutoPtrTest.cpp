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
 * AutoPtrTest.cpp
 *
 * FileId      : $Id: AutoPtrTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 6, 2012
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
#include <CxxAbb/RefCountedObj.h>
#include <gtest/gtest.h>

namespace {

class TestClass
{
public:
	TestClass()
	: ui_RefCount(1)
	{
		i_GlbCount++;
	}

	virtual ~TestClass()
	{
		i_GlbCount--;
	}

	unsigned int refAdd()
	{
		return (++ui_RefCount);
	}

	void refRem()
	{
		if ( (--ui_RefCount) == 0)
		{
//			std::cout << "Calling delete" << std::endl;
			delete this;
		}
	}

	int refCount() const
	{
		return ui_RefCount;
	}

//	virtual void PolyFunc()
//	{}

	static int i_GlbCount;
private:
	unsigned ui_RefCount;
};

int TestClass::i_GlbCount = 0;

class DerivedTestClass : public TestClass
{
public:
	DerivedTestClass()
	{}

	~DerivedTestClass()
	{}
};

class SomeOtherClass
{
public:
	SomeOtherClass()
	: ui_RefCount(1)
	{
	}

	virtual ~SomeOtherClass()
	{
	}

	unsigned int refAdd()
	{
		return (++ui_RefCount);
	}

	void refRem()
	{
		if ( (--ui_RefCount) == 0)
		{
//			std::cout << "Calling delete" << std::endl;
			delete this;
		}
	}

	int refCount() const
	{
		return ui_RefCount;
	}

private:
	unsigned ui_RefCount;
};

}

TEST(AutoPtrTest, Constructor)
{

	CxxAbb::AutoPtr<TestClass> varTC1 = new TestClass();

	EXPECT_FALSE(varTC1.isNull());
	EXPECT_FALSE(!varTC1);
	EXPECT_EQ(1, varTC1->refCount());

	CxxAbb::AutoPtr<TestClass> varTC2;

	EXPECT_TRUE(varTC2.isNull());
	EXPECT_THROW(varTC2->refCount(), CxxAbb::NullPointerException);
	EXPECT_THROW((*varTC2).refCount(), CxxAbb::NullPointerException);
	EXPECT_EQ(NULL,varTC2.get());
	EXPECT_TRUE(!varTC2);

}

TEST(AutoPtrTest, Destructor)
{
	ASSERT_EQ(0, TestClass::i_GlbCount);
	{
		CxxAbb::AutoPtr<TestClass> varTC1 = new TestClass();

		ASSERT_EQ(1, TestClass::i_GlbCount);

		CxxAbb::AutoPtr<TestClass> varTC2;

		ASSERT_EQ(1, TestClass::i_GlbCount);

		varTC2 = varTC1;

		ASSERT_EQ(1, TestClass::i_GlbCount);

		varTC2 = new TestClass();

		ASSERT_EQ(2, TestClass::i_GlbCount);

		CxxAbb::AutoPtr<TestClass> varTC3 = varTC1;

		ASSERT_EQ(2, TestClass::i_GlbCount);

		varTC1 = new TestClass(); // existing pointer released, but now owned by varTC3

		ASSERT_EQ(3, TestClass::i_GlbCount);

		varTC3 = new TestClass(); // existing pointer released (delete)

		ASSERT_EQ(3, TestClass::i_GlbCount);

		CxxAbb::AutoPtr<TestClass> varTC4 = varTC2;
		CxxAbb::AutoPtr<TestClass> varTC5 = varTC2;

		EXPECT_EQ(3, varTC2->refCount());
		EXPECT_EQ(3, varTC5->refCount());

		ASSERT_EQ(3, TestClass::i_GlbCount);

		TestClass * pTC = varTC2.get(); // no delete, just release ownership

		EXPECT_TRUE(pTC == varTC5.get());

		SUCCEED() << "End is here";

	}
	ASSERT_EQ(0, TestClass::i_GlbCount);
}

TEST(AutoPtrTest, Assignment)
{
	CxxAbb::AutoPtr<TestClass> varTC1 = new TestClass();
	CxxAbb::AutoPtr<TestClass> varTC2;

	EXPECT_EQ(1, varTC1->refCount());

	varTC2 = varTC1;

	EXPECT_EQ(2, varTC1->refCount());
	EXPECT_FALSE(varTC2.isNull());
	EXPECT_EQ(2, varTC2->refCount());

	varTC2 = new TestClass();

	EXPECT_EQ(1, varTC1->refCount());
	EXPECT_EQ(1, varTC2->refCount());

	varTC1 = varTC2;

	EXPECT_EQ(2, varTC1->refCount());
	EXPECT_EQ(2, varTC2->refCount());
}

TEST(AutoPtrTest, Operators)
{
	CxxAbb::AutoPtr<TestClass> varTC1;

	EXPECT_TRUE(varTC1.isNull());

	TestClass* pTC1 = new TestClass;
	TestClass* pTC2 = new TestClass;

	if (pTC2 < pTC1)
	{
		TestClass* pTmp = pTC1;
		pTC1 = pTC2;
		pTC2 = pTmp;
	}
	EXPECT_TRUE(pTC1 < pTC2);

	varTC1 = pTC1;
	CxxAbb::AutoPtr<TestClass> varTC2 = pTC2;
	CxxAbb::AutoPtr<TestClass> varTC3 = varTC1;
	CxxAbb::AutoPtr<TestClass> varTC4;

	EXPECT_TRUE (varTC1.get() == pTC1);
	EXPECT_TRUE (varTC1 == pTC1);
	EXPECT_TRUE (varTC2.get() == pTC2);
	EXPECT_TRUE (varTC2 == pTC2);
	EXPECT_TRUE (varTC3.get() == pTC1);
	EXPECT_TRUE (varTC3 == pTC1);

	EXPECT_TRUE (varTC1 == pTC1);
	EXPECT_TRUE (varTC1 != pTC2);
	EXPECT_TRUE (varTC1 < pTC2);
	EXPECT_TRUE (varTC1 <= pTC2);
	EXPECT_TRUE (varTC2 > pTC1);
	EXPECT_TRUE (varTC2 >= pTC1);

	EXPECT_TRUE (varTC1 == varTC3);
	EXPECT_TRUE (varTC1 != varTC2);
	EXPECT_TRUE (varTC1 < varTC2);
	EXPECT_TRUE (varTC1 <= varTC2);
	EXPECT_TRUE (varTC2 > varTC1);
	EXPECT_TRUE (varTC2 >= varTC1);

	varTC1 = pTC1;
	varTC2 = pTC2;
	varTC1.swap(varTC2);
	EXPECT_TRUE (varTC2.get() == pTC1);
	EXPECT_TRUE (varTC1.get() == pTC2);

	EXPECT_THROW(EXPECT_FALSE(varTC4->refCount() > 0), CxxAbb::NullPointerException);

	EXPECT_TRUE (!(varTC4 == varTC1));
	EXPECT_TRUE (!(varTC4 == varTC2));
	EXPECT_TRUE (varTC4 != varTC1);
	EXPECT_TRUE (varTC4 != varTC2);

	varTC4 = varTC1;
	EXPECT_TRUE (varTC4 == varTC1);
	EXPECT_TRUE (!(varTC4 != varTC1));

	EXPECT_TRUE (!(!varTC1));
	varTC1 = 0;
	EXPECT_TRUE (!varTC1);

	varTC2.reset();
	EXPECT_TRUE (!varTC2);

	CxxAbb::AutoPtr<TestClass> varTC5 = varTC4.duplicate();
	EXPECT_EQ(2, varTC4->refCount());
	EXPECT_EQ(2, varTC5->refCount());
}

TEST(AutoPtrTest, Casting)
{
	{
		CxxAbb::AutoPtr<TestClass> varTC1;
		CxxAbb::AutoPtr<DerivedTestClass> varDTC1(new DerivedTestClass());

		//varTC1 = varDTC1;
		varTC1 = new DerivedTestClass();
		//COUT_LOG() << "TypeId : " << typeid(*varTC1.get()).name();
		//COUT_LOG() << "TypeId : " << typeid(DerivedTestClass).name();
		// both pointed to same class
		EXPECT_TRUE(typeid(DerivedTestClass) == typeid(*varTC1.get()));

		varTC1 = varDTC1.cast<TestClass>(); // Not Null
		ASSERT_FALSE(varTC1.isNull());
		ASSERT_TRUE(typeid(TestClass*) == typeid(varTC1.get()));
		ASSERT_TRUE(typeid(DerivedTestClass) == typeid(*varTC1.get()));
		//COUT_LOG() << "TypeId : " << typeid(varTC1.get()).name();
		//COUT_LOG() << "TypeId : " << typeid(*varTC1.get()).name();
	}

	{
		CxxAbb::AutoPtr<TestClass> varTC1;
		CxxAbb::AutoPtr<DerivedTestClass> varDTC1(new DerivedTestClass());

		//varDTC1 = varTC1; // Build error
		varDTC1 = varTC1.cast<DerivedTestClass>(); // Null, TestClass is not a DerivedTestClass
		ASSERT_TRUE(varDTC1.isNull());
	}

	{
		CxxAbb::AutoPtr<TestClass> varTC1;
		CxxAbb::AutoPtr<DerivedTestClass> varDTC1(new DerivedTestClass());

		CxxAbb::AutoPtr<SomeOtherClass> varSOC1(new SomeOtherClass);
		varTC1 = varSOC1.cast<TestClass>();
		ASSERT_TRUE(varTC1.isNull());
	}

	{
		CxxAbb::AutoPtr<TestClass> varTC1(new DerivedTestClass);
		CxxAbb::AutoPtr<DerivedTestClass> varDTC1 = varTC1.unsafeCast<DerivedTestClass>();

		ASSERT_FALSE(varDTC1.isNull());
		//COUT_LOG() << "TypeId : " << typeid(varDTC1.get()).name();
		//COUT_LOG() << "TypeId : " << typeid(*varDTC1.get()).name();
	}
}

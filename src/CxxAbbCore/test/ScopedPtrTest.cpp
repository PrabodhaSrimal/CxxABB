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
 * ScopedPtrTest.cpp
 *
 * FileId      : $Id: ScopedPtrTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 5, 2012
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

}

TEST(ScopedPtrTest, Constructor)
{
	{
		CxxAbb::ScopedPtr<TestClass> varTC1(new TestClass());

		EXPECT_FALSE(varTC1.isNull());
		EXPECT_FALSE(!varTC1);
		EXPECT_EQ(1, varTC1->refCount());
	}

	{
		CxxAbb::ScopedPtr<TestClass> varTC2;
		EXPECT_TRUE(varTC2.isNull());
		EXPECT_THROW(varTC2->refCount(), CxxAbb::NullPointerException);
		EXPECT_THROW((*varTC2).refCount(), CxxAbb::NullPointerException);
		EXPECT_EQ(NULL,varTC2.get());
		EXPECT_TRUE(!varTC2);
	}

	{
		std::auto_ptr<TestClass> ap(new TestClass());
		CxxAbb::ScopedPtr<TestClass> varTC1(ap);

	}
}
 
TEST(ScopedPtrTest, Destructor)
{
	ASSERT_EQ(0, TestClass::i_GlbCount);
	{
		CxxAbb::ScopedPtr<TestClass> varTC1(new TestClass());

		ASSERT_EQ(1, TestClass::i_GlbCount);
	}
	ASSERT_EQ(0, TestClass::i_GlbCount);
	{
		CxxAbb::ScopedPtr<TestClass> varTC1(new TestClass());
		ASSERT_EQ(1, TestClass::i_GlbCount);
		varTC1.reset();
		ASSERT_EQ(0, TestClass::i_GlbCount);
	}
	ASSERT_EQ(0, TestClass::i_GlbCount);
	{
		CxxAbb::ScopedPtr<TestClass> varTC1(new TestClass());
		ASSERT_EQ(1, TestClass::i_GlbCount);
		TestClass * tc = new TestClass();
		ASSERT_EQ(2, TestClass::i_GlbCount);
		varTC1.reset(tc);
		ASSERT_EQ(1, TestClass::i_GlbCount);
	}
	ASSERT_EQ(0, TestClass::i_GlbCount);
}

TEST(ScopedPtrTest, Operators)
{
	CxxAbb::ScopedPtr<TestClass> varTC1(new TestClass());
}

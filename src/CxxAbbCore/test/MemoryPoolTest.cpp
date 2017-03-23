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
 * MemoryPoolTest.cpp
 *
 * FileId      : $Id: MemoryPoolTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 9, 2012
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
#include <CxxAbb/MemoryPool.h>
#include <gtest/gtest.h>

namespace
{

class Stock
{
public:
	char z_Symbol[32];
	float f_LastPrice;
	unsigned int ui_LastQty;
	CxxAbb::UInt64 ui_Volume;
	double d_Turnover;
	float f_BestAsk;
	unsigned int ui_BestAskQty;
	float f_BestBid;
	unsigned int ui_BestBidQty;
};

}

TEST(MemoryPoolTest, Allocation)
{
	std::size_t blocksize = sizeof(Stock);
	CxxAbb::MemoryPool pool(blocksize);

	ASSERT_TRUE(pool.Allocated() == 0);
	ASSERT_TRUE(pool.MaxBlocks() == 0);
	ASSERT_TRUE(pool.Available() == 0);

	CxxAbb::MemoryPool pool1(blocksize, 10, 0);

	ASSERT_EQ(pool1.Allocated(), 0);
	ASSERT_EQ(pool1.MaxBlocks(), 10);
	ASSERT_EQ(pool1.Available(), 0);

	std::vector<Stock*> ptrs;
	for (int i = 0; i < 10; ++i)
	{
		Stock * s = static_cast<Stock*>(pool1.Get());
		ptrs.push_back(s);
		ASSERT_EQ (pool1.Allocated(), i + 1);
		ASSERT_EQ (pool1.Available(), 0);
	}

	ASSERT_THROW(pool1.Get(), CxxAbb::OutOfMemoryException);

	int av = 0;
	for (std::vector<Stock*>::iterator it = ptrs.begin(); it != ptrs.end(); ++it)
	{
		pool1.Release(*it);
		++av;
		ASSERT_EQ (pool1.Available(), av);
	}

	CxxAbb::MemoryPool pool2(32, 10, 5);
	ASSERT_TRUE (pool2.Available() == 5);
	ASSERT_TRUE (pool2.BlockSize() == 32);
	ASSERT_TRUE (pool2.Allocated() == 5);
}
 

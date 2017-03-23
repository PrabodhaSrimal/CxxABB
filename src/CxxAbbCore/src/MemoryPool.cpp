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
 * MemoryPool.cpp
 *
 * FileId      : $Id: MemoryPool.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Simple memory pool for preallocated/dynamic allocated applications
 *
 */


#include <CxxAbb/MemoryPool.h>
#include <CxxAbb/Exception.h>
#include <CxxAbb/Sys/ScopedLock.h>
#include <CxxAbb/Debug.h>

namespace CxxAbb
{

MemoryPool::MemoryPool(std::size_t _tBlockSize, int _iMaxBlocks /*= 0*/, int _iPreAlloced /*= 0*/)
	: t_BlockSize(_tBlockSize),
	  i_MaxBlocks(_iMaxBlocks),
	  i_AllocatedBlocks(_iPreAlloced)
{
	ASSERT (i_MaxBlocks == 0 || i_MaxBlocks >= _iPreAlloced);
	ASSERT (_iPreAlloced >= 0 && i_MaxBlocks >= 0);

	int i = INIT_ALLOCATED;
	if (_iPreAlloced > i)
		i = _iPreAlloced;
	if (i_MaxBlocks > 0 && i_MaxBlocks < i)
		i = i_MaxBlocks;
	m_Memblocks.reserve(i);
	for (int j = 0; j < _iPreAlloced; ++j)
	{
		m_Memblocks.push_back(new char[t_BlockSize]);
	}
}

MemoryPool::~MemoryPool()
{
	for (MemBlocks::iterator it = m_Memblocks.begin(); it != m_Memblocks.end(); ++it)
	{
		delete [] *it;
	}
}

void* MemoryPool::Get()
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(mtx_Lock);

	if (m_Memblocks.empty())
	{
		if (i_MaxBlocks == 0 || i_AllocatedBlocks < i_MaxBlocks)
		{
			++i_AllocatedBlocks;
			return new char[t_BlockSize];
		}
		else throw CxxAbb::OutOfMemoryException("MemoryPool max limit reached");
	}
	else
	{
		char* ptr = m_Memblocks.back();
		m_Memblocks.pop_back();
		return ptr;
	}
}

void MemoryPool::Release(void* _pBlock)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(mtx_Lock);

	m_Memblocks.push_back(reinterpret_cast<char*>(_pBlock));
}

}  /* namespace CxxAbb */

 

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
 * MemoryPool.h
 *
 * FileId      : $Id: MemoryPool.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Nov 8, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Simple memory pool for preallocated/dynamic allocated applications
 *
 */

#ifndef CXXABB_CORE_MEMORYPOOL_H_
#define CXXABB_CORE_MEMORYPOOL_H_


#include <CxxAbb/Core.h>
#include <CxxAbb/NonCopyable.h>
#include <CxxAbb/Sys/Mutex.h>
#include <vector>

namespace CxxAbb
{

/** @brief Simple memory pool for simple needs
 * - Preallocated memory pool
 * - With max pool limit
 *
 * Recommended for use with data structures with same size (BlockSize)
 */
class CXXABB_API MemoryPool : private CxxAbb::NonCopyable
{
	typedef std::vector<char*> MemBlocks;
public:
	/** @brief Create memory pool
	 * @param _tBlockSize Block size in bytes of memory pool
	 * @param _iMaxBlocks Max limit of pool (_tBlockSize*_iMaxBlocks Bytes); if zero unlimited
	 * @param _iPreAlloced Preallocated blocks
	 */
	MemoryPool(std::size_t _tBlockSize, int _iMaxBlocks = 0, int _iPreAlloced = 0);

	~MemoryPool();

	/** @brief Get memory block from pool
	 * Has the same semantic as *malloc()*
	 */
	void * Get();

	/** @brief Release a memory block and put it back to pool
	 *  Has the same semantic as *free()*
	 */
	void Release(void * _pBlock);

	std::size_t BlockSize() const
	{
		return t_BlockSize;
	}

	unsigned int MaxPoolSize() const
	{
		return t_BlockSize * i_MaxBlocks;
	}

	int MaxBlocks() const
	{
		return i_MaxBlocks;
	}

	int Allocated() const
	{
		return i_AllocatedBlocks;
	}

	int Available() const
	{
		return m_Memblocks.size();
	}

private:
	MemoryPool();

	MemBlocks m_Memblocks;
	std::size_t t_BlockSize;
	int i_MaxBlocks;
	int i_AllocatedBlocks;
	CxxAbb::Sys::FastMutex mtx_Lock;

	enum
	{
		INIT_ALLOCATED = 128
	};
};

}  /* namespace CxxAbb */


#endif /* CXXABB_CORE_MEMORYPOOL_H_ */

#include "PoolAllocator.h"

namespace ge 
{
	constexpr usize blockSize = 16;

	void* PoolAllocator::allocateBlock(usize length)
	{
		return new char[length];
	}

	void PoolAllocator::freeBlock(void* a, usize length)
	{
		delete[] ((char*)a);
	}

}

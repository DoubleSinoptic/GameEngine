#pragma once
#ifndef __POOLALLOCATOR_H__
#define __POOLALLOCATOR_H__

#include "./Core.h"

namespace ge
{
	class PoolAllocator 
	{
	public:
		static void* allocateBlock(usize length);
		static void  freeBlock(void*, usize);

		template<typename T, typename... Args>
		static T* allocate(Args&&... args) 
		{
#ifdef NDEBUG
			void* data = allocateBlock(sizeof(T));
			T* object = new(data) T(std::forward<Args>(args)...);
			return object;
#else
			return new T(std::forward<Args>(args)...);
#endif
		}

		template<typename T>
		static void free(T* object) 
		{
#ifdef NDEBUG
			freeBlock(const_cast<void*>(reinterpret_cast<const void*>(object)), sizeof(T));
#else
			delete object;
#endif		
		}

	};
}

#endif
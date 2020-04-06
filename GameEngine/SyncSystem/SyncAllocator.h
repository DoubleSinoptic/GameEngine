#pragma once
#include "Core/Core.h"
#include <vector>

namespace ge
{

	class GE_EXPORT SyncAllocator
	{
		Vector<Vector<byte>> m_frames;
		byte* m_cur;
		usize				 m_offset;
		usize				 m_max;
	public:
		SyncAllocator();

		void resetBack();
		void reset();

		template<typename T>
		static void destroy(T* o)
		{
			o->~T();
		}

		void* allocate(usize n);
		
		template<typename T>
		T* allocate()
		{
			auto ptr = reinterpret_cast<T*>(allocate(sizeof(T)));
			new(ptr) T();
			return ptr;
		}

		
	};

}
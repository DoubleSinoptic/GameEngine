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

		void* allocate(usize n);
		void* alignedAllocate(usize size, usize alignment);

		template<typename T>
		T* allocate()
		{
			return reinterpret_cast<T*>(allocate(sizeof(T)));
		}

		template<typename T>
		T* alignedAllocate()
		{
			return reinterpret_cast<T*>(alignedAllocate(sizeof(T)));
		}
	};

}
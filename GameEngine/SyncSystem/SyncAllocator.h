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
		void* alignedAllocate(usize size, usize alignment = 16);

		template<typename T>
		T* allocate()
		{
			return reinterpret_cast<T*>(allocate(sizeof(T)));
		}

		template<typename T>
		T* alignedAllocate(usize allign = 16)
		{
			return reinterpret_cast<T*>(alignedAllocate(sizeof(T), allign));
		}
	};

}
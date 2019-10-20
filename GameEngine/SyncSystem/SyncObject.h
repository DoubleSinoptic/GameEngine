#pragma once
#include "Core/Core.h"
#include "SyncAllocator.h"

namespace ge
{

	namespace rt
	{
		class GE_EXPORT SyncObject
		{
		public:
			virtual ~SyncObject() = default;
			virtual void initialize() = 0;
			virtual void sync(void* data, uint32 flags) = 0;
			virtual void destroy()
			{
				delete this;
			}
		};
	}

	using RtSyncObject = rt::SyncObject;

	constexpr usize InvalidDirtyIndex = usize(-1);

	template<typename T>
	struct SyncTag {};

	class GE_EXPORT SyncObject
	{
		friend class    SyncManager;
		usize			m_dirtyIndex;
		uint32			m_dirtyFlags;
		rt::SyncObject* m_rt;
		void			initialize();
	public:
		template<typename T>
		SyncObject(SyncTag<T> fn) :
			m_rt(new T()),
			m_dirtyIndex(InvalidDirtyIndex),
			m_dirtyFlags(0)
		{
			initialize();
		}

		virtual ~SyncObject();

		void markAsDirty(uint32 flags);

		void makeClean();

		constexpr rt::SyncObject* getRtObject() const noexcept
		{
			return m_rt;
		}

		template<typename T>
		constexpr T* getRtObjectT() const noexcept
		{
			return static_cast<T*>(m_rt);
		}

		virtual void* sync(SyncAllocator* allocator, uint32 flags) const = 0;
	};

}
#pragma once
#include "Core/Core.h"
#include "Core/CommandQueue.h"
#include "Core/ObjectPool.h"
#include "SyncAllocator.h"
#include "Core/Thread.h"
namespace ge
{

	namespace rt {
		class SyncObject;
	}
	class SyncObject;
	class NONDTR_STATIC GE_EXPORT SyncManager
	{
		struct SyncInfo
		{
			constexpr SyncInfo(void* Vdata, rt::SyncObject* Vobject, uint32	Vflags) :
				data(Vdata),
				flags(Vflags),
				object(Vobject)
			{}
			void* data;
			rt::SyncObject* object;
			uint32			flags;
		};

		ObjectPool<SyncAllocator>		  m_allocatorsPool;
		ObjectPool<Vector<SyncInfo>>      m_objectsPool;
		CommandQueue					  m_queue;
		Vector<SyncObject*>               m_dirtyObjects;
		Thread							  m_workThread;
		void							  workThread();
	public:

		SyncManager();
		~SyncManager();

		static void setCurrentSyncManager(Ptr<SyncManager> m_current);
		static SyncManager& instance();

		void collect();

		constexpr Vector<SyncObject*>& getDirtyObjects() noexcept {
			return m_dirtyObjects;
		}

		constexpr const Vector<SyncObject*>& getDirtyObjects() const noexcept {
			return m_dirtyObjects;
		}

		constexpr const CommandQueue& getQueue() const
		{
			return m_queue;
		}

		constexpr CommandQueue& getQueue()
		{
			return m_queue;
		}

		void sync();
		void playback();
	};

}
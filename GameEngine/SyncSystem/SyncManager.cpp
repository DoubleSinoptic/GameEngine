#include "SyncManager.h"
#include "SyncObject.h"

namespace ge
{

	SyncManager globlSyncManager;

	void SyncManager::collect()
	{
		m_objectsPool.collect(4);
		m_allocatorsPool.collect(4);
	}

	void SyncManager::sync()
	{
		auto allocatorsPoolSmart = m_allocatorsPool.allocate([]() { return snew<SyncAllocator>(); });
		SyncAllocator* allocator = &allocatorsPoolSmart.operator*();
		auto objectsSmart = m_objectsPool.allocate([]() { return snew<Vector<SyncInfo>>(); });
		Vector<SyncInfo>* objects = &objectsSmart.operator*();

		for (auto x : m_dirtyObjects)
		{
			objects->push_back({ x->sync(allocator, x->m_dirtyFlags), x->getRtObject(), x->m_dirtyFlags });
			x->makeClean();
		}
		m_dirtyObjects.clear();

		m_queue.queue([=]()
			{
				for (auto x : *objects)
				{
					x.object->sync(x.data, x.flags);
				}
				objects->clear();
				m_objectsPool.free(objectsSmart);
				allocator->reset();
				m_allocatorsPool.free(allocatorsPoolSmart);
			});
	}

	void SyncManager::playback()
	{
		m_queue.playback();
	}

	SyncManager& SyncManager::instance()
	{
		return globlSyncManager;
	}

}
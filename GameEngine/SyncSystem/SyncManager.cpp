#include "SyncManager.h"
#include "SyncObject.h"

namespace ge
{

	Ptr<SyncManager> currentSyncManager;

	void SyncManager::workThread()
	{
		Thread::setThreadMarker(utf8("Render Thread"));
		while (!Thread::isClosed())
		{
			playback();
		}
		playback();
	}

	SyncManager::SyncManager() :
		m_workThread([=]() { workThread(); })
	{}

	SyncManager::~SyncManager()
	{
		m_queue.queue([=]() {
			m_workThread.close();
		});
		m_workThread.join();
	}

	void SyncManager::setCurrentSyncManager(Ptr<SyncManager> m_current)
	{
		currentSyncManager = m_current;
	}

	SyncManager& SyncManager::instance()
	{
		return *currentSyncManager;
	}

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
}
#include "SyncObject.h"
#include "SyncManager.h"

namespace ge
{
	void SyncObject::initialize()
	{
		SyncManager::instance().getQueue().queue(m_rt, &rt::SyncObject::initialize);
	}

	void SyncObject::destroy()
	{
		if (m_rt) 
		{
			SyncManager::instance().getQueue().queue(m_rt, &rt::SyncObject::destroy);
			m_rt = nullptr;
		}			
	}

	SyncObject::~SyncObject()
	{
		if (m_dirtyIndex != (size_t)-1)
		{
			auto& dirtyObjects = SyncManager::instance().getDirtyObjects();
			dirtyObjects.back()->m_dirtyIndex = m_dirtyIndex;
			dirtyObjects[m_dirtyIndex] = dirtyObjects.back();
			dirtyObjects.pop_back();
		}
		if (m_rt)
			SyncManager::instance().getQueue().queue(m_rt, &rt::SyncObject::destroy);
	}

	void SyncObject::markAsDirty(uint32 flags)
	{
		m_dirtyFlags |= flags;
		if (m_dirtyIndex == InvalidDirtyIndex)
		{
			m_dirtyIndex = SyncManager::instance().getDirtyObjects().size();
			SyncManager::instance().getDirtyObjects().push_back(this);
		}
	}

	void SyncObject::makeClean()
	{
		m_dirtyIndex = InvalidDirtyIndex;
		m_dirtyFlags = 0;
	}

}
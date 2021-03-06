#include "EngineApplication.h"
#include "SyncSystem/SyncManager.h"
#include "Core/Time.h"
#include "Core/Debug.h"
#include "GameObject.h"
#include "Physics/Physics.h"

namespace ge
{
	EngineApplication::EngineApplication() :
		m_syncManager(snew<SyncManager>()),
		m_pool(snew<ThreadPool>()),
		m_manager(snew<GameObjectManager>()),
		m_config(snew<Config>(u"settings.bcfg")),
		m_physics(snew<Physics>(0)),
		m_renderFinished(true)
	{
		Thread::setThreadMarker(utf8("Main Thread"));
		ThreadPool::setCurrentThreadPool(m_pool);
		SyncManager::setCurrentSyncManager(m_syncManager);
		Physics::setCurrentPhysics(m_physics);
		GameObjectManager::setCurrentObjectManager(m_manager);
		Config::setCurrentConfig(m_config);
	
		int64 tickRate = m_config->getValueInt64(u"tickRate", 60);
		m_tickLocker.setDelta(tickRate ? (1000000 / tickRate) : 0);

		m_renderManager = snew<RenderManager>();
		RenderManager::setCurrentRenderManager(m_renderManager);
	}


	EngineApplication::~EngineApplication()
	{
		GameObject::clearScene();
		m_pool->barrier(BT_STRONG);
		RenderManager::setCurrentRenderManager(nullptr);
		m_renderManager = nullptr;

		ThreadPool::setCurrentThreadPool(nullptr);
		SyncManager::setCurrentSyncManager(nullptr);
		Physics::setCurrentPhysics(nullptr);
		GameObjectManager::setCurrentObjectManager(nullptr);
		Config::setCurrentConfig(nullptr);
		RenderManager::setCurrentRenderManager(nullptr);
	}

	void EngineApplication::run()
	{
		uint64 point = 0;
		while (!m_manager->isExited())
		{
			m_tickLocker.lock();
			uint64 delta = Time::deltaTick(point);

			constexpr uint64 fixedDelta = 1000000 / 60;
			constexpr uint64 maximumDelta = fixedDelta / 2 + fixedDelta;

			uint64 total = delta;
			while (total >= maximumDelta) 
			{
				m_manager->fixedUpdate(fixedDelta * 0.000001);
				total -= fixedDelta;
			}
			m_manager->fixedUpdate(total * 0.000001);
			
			m_manager->update(delta * 0.000001);

			disptach();

			SyncManager::instance().sync();
			SyncManager::instance().collect();
			RenderManager::instance().render();
			
			{
				std::unique_lock<std::mutex> _(m_renderLock);
				while (!m_renderFinished)
					m_renderVariable.wait(_);	
				m_renderFinished = false;
			}
			
			SyncManager::instance().getQueue().queue([=]()
			{
				std::unique_lock<std::mutex> _(m_renderLock);
				m_renderFinished = true;
				m_renderVariable.notify_one();
			});
		}
		GameObject::clearScene();
	}
}

#include "EngineApplication.h"
#include "SyncSystem/SyncManager.h"
#include "Core/Time.h"
#include "Core/Debug.h"
#include "GameObject.h"

namespace ge
{
	EngineApplication::EngineApplication() :
		m_pool(snew<ThreadPool>()),
		m_manager(snew<GameObjectManager>()),
		m_config(snew<Config>(u"settings.bcfg")),
		m_renderFinished(true)
	{
		ThreadPool::setCurrentThreadPool(m_pool);
		GameObjectManager::setCurrentObjectManager(m_manager);
		Config::setCurrentConfig(m_config);

		int64 tickRate = m_config->getValueInt64(u"tickRate", 60);
		m_tickLocker.setDelta(tickRate ? (1000000 / tickRate) : 0);

		m_renderThread = snew<Thread>([=]() { renderThread(); });
	}

	void EngineApplication::renderThread()
	{
		while (!Thread::isClosed())
		{
			SyncManager::instance().playback();
		}
	}

	void EngineApplication::render()
	{
	}

	EngineApplication::~EngineApplication()
	{
		ThreadPool::setCurrentThreadPool(nullptr);
		GameObjectManager::setCurrentObjectManager(nullptr);
		Config::setCurrentConfig(nullptr);
	}

	void EngineApplication::run()
	{
		uint64 point = 0;
		while (!m_manager->isExited())
		{
			m_tickLocker.lock();
			uint64 delta = Time::deltaTick(point);
			Debug::log("{0}", delta);
			m_manager->fixedUpdate(delta);

			m_manager->update(delta);

			SyncManager::instance().sync();
			SyncManager::instance().collect();

			SyncManager::instance().getQueue().queue([=]()
			{
				render();
			});
			
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

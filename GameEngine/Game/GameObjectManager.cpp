#include "GameObjectManager.h"
#include "Component.h"
#include "Physics/Physics.h"

namespace ge 
{
	Ptr<GameObjectManager> mainManager;

	void GameObjectManager::markComponentsDirty()
	{
		m_componentsDirty = true;
	}

	GameObjectManager::GameObjectManager() :
		m_componentsDirty(false),
		m_fixedDelta(0.0),
		m_delta(0.0),
		m_isPlayed(true),
		m_isPlayedOld(true),
		m_isExited(false)
	{}

	void GameObjectManager::setCurrentObjectManager(Ptr<GameObjectManager> manager)
	{
		mainManager = manager;
	}

	GameObjectManager& GameObjectManager::instance()
	{
		return *mainManager;
	}

	bool componentComporator(Component* a, Component* b)
	{
		return a->getPriority() > b->getPriority();
	}

	bool GameObjectManager::isExited() const noexcept
	{
		return m_isExited;
	}

	void GameObjectManager::exit()
	{
		m_isExited = true;
	}

	void GameObjectManager::play()
	{
		m_isPlayed = true;
	}

	void GameObjectManager::stop()
	{
		m_isPlayed = false;
	}

	void GameObjectManager::update(scalar dt)
	{
		m_delta = dt;

		for (usize i = 0; i < m_components.size(); i++) {
			m_components[i]->update();
		}

		//auto& currInvoke = _M_invokeTable[_M_firstInvoke];
		//_M_firstInvoke = !_M_firstInvoke;
		//for (USize i = 0; i < currInvoke.Length(); i++) {
		//	auto& v = currInvoke[i];
		//	if (v.component) {
		//		v.component->_M_invokeId = UIntMax;
		//		v.component->OnInvoke(v.invokeId);
		//	}
		//}
		//currInvoke.Clear();
	}

	void GameObjectManager::fixedUpdate(scalar dt)
	{
		if (m_isPlayed != m_isPlayedOld)
		{
			m_isPlayedOld = m_isPlayed;
			if (m_isPlayed)
				for (usize i = 0; i < m_components.size(); i++) 
					m_components[i]->awake();
			else
				for (usize i = 0; i < m_components.size(); i++) 
					m_components[i]->sleep();			
		}
		
		m_fixedDelta = dt;

		for (usize i = 0; i < m_components.size(); i++) {
			m_components[i]->fixedUpdate();
		}
		if (m_componentsDirty) {
			std::sort(m_components.begin(), m_components.end(), componentComporator);
			for (usize i = 0; i < m_components.size(); i++)
				m_components[i]->setPlace(i);
			m_componentsDirty = false;
		}
		
		Physics::instance().update(dt, false);
	}
}


#pragma once
#ifndef __GAMEOBJECTMANAGER_H__
#define __GAMEOBJECTMANAGER_H__

#include "Core/Core.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace ge
{
	class Component;
	class GameObject;
	class GameObjectManager 
	{

		scalar				m_delta;
		scalar				m_fixedDelta;
		Vector<Component*>  m_components;
		Vector<GameObject*> m_gameObjects;
		bool				m_componentsDirty;
		bool				m_isPlayed;
		bool				m_isPlayedOld;
		bool				m_isExited;
	public:
		void markComponentsDirty();

		GameObjectManager();

		constexpr Vector<Component*>& components()
		{
			return m_components;
		}

		constexpr Vector<GameObject*>& gameObjects()
		{
			constexpr size_t s = 3139 / 2;
			return m_gameObjects;
		}
		static void setCurrentObjectManager(Ptr<GameObjectManager> manager);
		static GameObjectManager& instance();

		bool isExited() const noexcept;
		void exit();

		void play();
		void stop();

		void update(scalar dt);
		void fixedUpdate(scalar dt);
	};
}

#endif
#pragma once
#ifndef __ENGINEAPPLICATION_H__
#define __ENGINEAPPLICATION_H__

#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/ThreadPool.h"
#include "Core/TickLocker.h"
#include "Core/Thread.h"
#include "GameObjectManager.h"
#include "Physics/Physics.h"

namespace ge
{
	class EngineApplication 
	{
		TickLocker			    m_tickLocker;
		Ptr<GameObjectManager>  m_manager;
		Ptr<Config>			    m_config;
		Ptr<ThreadPool>		    m_pool;
		Ptr<Thread>			    m_renderThread;
		Ptr<Physics>		    m_physics;
		std::condition_variable m_renderVariable;
		std::mutex				m_renderLock;
		bool					m_renderFinished;
		void renderThread();
		void render();
	public:

		void bind();

		EngineApplication();
		~EngineApplication();

		void run();
	};
}

#endif
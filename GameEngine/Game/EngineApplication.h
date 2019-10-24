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
#include "Rendering/GpuPool.h"
#include "SyncSystem/SyncManager.h"

namespace ge
{
	class EngineApplication 
	{
		TickLocker			    m_tickLocker;		
		Ptr<SyncManager>		m_syncManager;
		Ptr<ThreadPool>		    m_pool;
		Ptr<GameObjectManager>  m_manager;
		Ptr<Config>			    m_config;		
		Ptr<Thread>			    m_renderThread;
		Ptr<Physics>		    m_physics;
		Ptr<GpuPool>			m_gpuPool;
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
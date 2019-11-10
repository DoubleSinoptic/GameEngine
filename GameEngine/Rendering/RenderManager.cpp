#include "RenderManager.h"
#include "../SyncSystem/SyncManager.h"

#include "../RenderAPI/Vulkan/VulkanGpuContext.h"

namespace ge 
{
	namespace rt 
	{	
		Ptr<RenderManager> currentRenderManagerRt;

		RenderManager& RenderManager::instance()
		{
			return *currentRenderManagerRt;
		}

		void RenderManager::setCurrentRenderManager(Ptr<RenderManager> manager)
		{
			currentRenderManagerRt = manager;
		}

		void RenderManager::initialize()
		{
			m_gpuContext = snew<VulkanGpuContext>();
			m_gpuPool = snew<GpuPool>();
		}

		void RenderManager::prepareToDestory()
		{
		}

		RenderManager::RenderManager()
		{
		}

		RenderManager::~RenderManager()
		{
		}

		void RenderManager::render() 
		{
		}
	}

	Ptr<RenderManager> currentRenderManager;

	RenderManager::RenderManager() :
		m_rt(snew<rt::RenderManager>())
	{
		SyncManager::instance().getQueue().queue([=]() {
			m_rt->initialize();
		});
	}

	RenderManager::~RenderManager()
	{
		SyncManager::instance().getQueue().queue([=]() {
			m_rt->prepareToDestory();
		});
	}

	void RenderManager::render()
	{
		SyncManager::instance().getQueue().queue([=]() {
			m_rt->render();
		});
	}

	RenderManager& RenderManager::instance()
	{
		return *currentRenderManager;
	}

	void RenderManager::setCurrentRenderManager(Ptr<RenderManager> manager)
	{
		currentRenderManager = manager;
		SyncManager::instance().getQueue().queue([=]() {
			rt::RenderManager::setCurrentRenderManager(manager->m_rt);
		});
	}
}

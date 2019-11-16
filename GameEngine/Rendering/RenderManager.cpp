#include "RenderManager.h"
#include "../SyncSystem/SyncManager.h"

#include "../RenderAPI/Vulkan/VulkanGpuContext.h"

namespace ge 
{
	namespace rt 
	{	
		RenderManager* currentRenderManagerRt = nullptr;

		RenderManager& RenderManager::instance()
		{
			return *currentRenderManagerRt;
		}

		void RenderManager::setCurrentRenderManager(RenderManager* manager)
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
		m_rt(new rt::RenderManager())
	{
		SpinLock wait(true);
		SyncManager::instance().getQueue().queue([&]() {
			m_rt->initialize();
			wait.unlock();
		});
		wait.lock();
	}

	RenderManager::~RenderManager()
	{
		SpinLock wait(true);
		SyncManager::instance().getQueue().queue([&]() {
			m_rt->prepareToDestory();
			delete m_rt;
			m_rt = nullptr;
			wait.unlock();
		});
		wait.lock();
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
		if (manager)
			SyncManager::instance().getQueue().queue([=]() {
				rt::RenderManager::setCurrentRenderManager(manager->m_rt);
			});
	}
}

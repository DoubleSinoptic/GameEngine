#include "RenderManager.h"
#include "../SyncSystem/SyncManager.h"
#include "Camera.h"
#include "../Core/Any.h"
//#include "../RenderAPI/Vulkan/VulkanGpuContext.h"
#include "../Window/WindowManager.h"
namespace ge 
{
	CmPtr<IContext> __cdecl CreateD3D11GraphicsContext(const Any& v);
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
			m_gpuContext = CreateD3D11GraphicsContext(WindowManager::instance().mainWindow()->getNativeHandle());
		
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
			for (auto x : m_camers) {
				if (x->cameraState & CAMERA_STATE_RENDER_BIT) {
					if (x->cameraState & CAMERA_STATE_MAIN_BIT) {
						ITextureRef tex = m_gpuContext->nextDrawable();
						x->render(tex);
						m_gpuContext->present(tex);
						uint32 we = 0, he = 0;
						WindowManager::instance().mainWindow()->getSize(we, he);
						if (we != tex->getDesc().width || he != tex->getDesc().heigh)
							m_gpuContext->resizeDrawable(we, he);
					} else {
						geAssertFalse("not support now");
					}


				}
			}
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

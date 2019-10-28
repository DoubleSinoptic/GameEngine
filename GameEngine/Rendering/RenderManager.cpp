#include "RenderManager.h"
namespace ge 
{
	namespace rt 
	{	
		Ptr<RenderManager> currentRenderManager;


		RenderManager& RenderManager::instance()
		{
			return *currentRenderManager;
		}

		void RenderManager::setCurrentRenderManager(Ptr<RenderManager> manager)
		{
			currentRenderManager = manager;
		}
	}
}

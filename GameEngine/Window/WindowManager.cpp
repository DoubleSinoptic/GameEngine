#include "Monitor.h"
#include "WindowManager.h"

namespace ge 
{
	Ptr<WindowManager> currentManager;
	WindowManager& WindowManager::instance()
	{
		return *currentManager;
	}
	void WindowManager::setCurrentWindowManager(Ptr<WindowManager> manager)
	{
		currentManager = manager;
	}
}
#pragma once
#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "Core/Core.h"
#include "Window.h"

namespace ge
{
	class WINDOW_DESC 
	{
		String			title;
		Ptr<Monitor>	monitor;
		int32			x;
		int32			y;
		VideoMod&		videoMod;
	};

	class WindowManager
	{
	protected:
		WindowManager() {}
	public:
		static WindowManager& instance();
		static void setCurrentWindowManager(Ptr<WindowManager> manager);

		virtual ~WindowManager() = default;
		virtual Ptr<Monitor>			primaryMonitor() = 0;
		virtual Vector<Ptr<Monitor>>	allMonitors() = 0;
		virtual Ptr<Window>				create(const WINDOW_DESC& desc) = 0;
		virtual void					dispatchMessages() = 0;
	};
}

#endif
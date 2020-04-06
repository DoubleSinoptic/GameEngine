#pragma once
#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "Core/Core.h"
#include "Window.h"

namespace ge
{
	struct WINDOW_DESC 
	{
		Ptr<Window>		parent;
		String			title;
		Ptr<Monitor>	monitor;
		VideoMod		videoMod;
	};

	class WindowManager
	{
		Ptr<Window> m_main;
	protected:
		WindowManager() {}
	public:
		static WindowManager& instance();
		static void setCurrentWindowManager(Ptr<WindowManager> manager);

		void setMainWindow(const Ptr<Window>& wnd) {
			m_main = wnd;
		}

		Ptr<Window> mainWindow() const {
			return m_main;
		}
	
		virtual ~WindowManager() = default;
		virtual Ptr<Monitor>			primaryMonitor() = 0;
		virtual Vector<Ptr<Monitor>>	allMonitors() = 0;
		virtual Ptr<Window>				create(const WINDOW_DESC& desc) = 0;
		virtual void					dispatchMessages() = 0;
	};
}

#endif
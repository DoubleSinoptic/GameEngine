#include "GlfwWindowManager.h"
#include "Core/StringFormat.h"
#include "Core/Library.h"

#include <Windows.h>

namespace ge 
{

	class GlfwMonitor : public Monitor
	{
		GlfwWindowManager*	m_instance;
		glfw::GLFWmonitor*		m_monitor;
	public:
		GlfwMonitor(glfw::GLFWmonitor* monitor, GlfwWindowManager* instance) :
			m_monitor(monitor),
			m_instance(instance)
		{
		}

		glfw::GLFWmonitor* native() const noexcept 
		{
			return m_monitor;
		}

		virtual String getName() const override
		{
			return utf8(m_instance->glfwGetMonitorName(m_monitor));
		}

		virtual void getPhysicsSize(uint32& w, uint32& h) const override
		{
			int x, y;
			m_instance->glfwGetMonitorPhysicalSize(m_monitor, &x, &y);
			w = x;
			h = y;
		}

		virtual void getWorkArea(int32& x, int32& y, uint32& w, uint32& h) const override
		{
			int tx, ty;
			m_instance->glfwGetMonitorWorkarea(m_monitor, &x, &y, &tx, &ty);
			w = x;
			h = y;
		}

		virtual void getLocaiton(int32& w, int32& h) const override
		{
			int x, y;
			m_instance->glfwGetMonitorPos(m_monitor, &x, &y);
			w = x;
			h = y;
		}

		virtual void getScale(float& w, float& h) const override
		{
			m_instance->glfwGetMonitorContentScale(m_monitor, &w, &h);
		}

		virtual Vector<VideoMod> getVideoModes() const override
		{
			int count;
			auto result = m_instance->glfwGetVideoModes(m_monitor, &count);
			Vector<VideoMod> modes;
			for (int i = 0; i < count; i++)
				modes.push_back({ result[i].refreshRate, uint32(result[i].width), uint32(result[i].height) });
			return modes;
		}

		virtual VideoMod getCurrentVideoMod() const override
		{
			auto vd = m_instance->glfwGetVideoMode(m_monitor);
			return {vd->refreshRate, uint32(vd->width), uint32(vd->height) };;
		}

	};

	class GlfwWindow : public Window
	{
		String					m_title;
		GlfwWindowManager*		m_instance;
		glfw::GLFWwindow*		m_window;
	public:
		~GlfwWindow() 
		{	
			m_instance->glfwDestroyWindow(m_window);
		}

		glfw::GLFWwindow* native() const noexcept
		{
			return m_window;
		}

		virtual Any getNativeHandle() const noexcept override
		{ 
			return (::HWND)m_instance->glfwGetWin32Window(m_window);
		}

		GlfwWindow(const WINDOW_DESC& desc, GlfwWindowManager* manager) :
			m_instance(manager),
			m_window(nullptr),
			m_title(desc.title)
		{
			glfw::GLFWwindow* glfwParentWindow = nullptr;
			glfw::GLFWmonitor* glfwMonitor = nullptr;
			if (desc.monitor) 
				glfwMonitor = std::static_pointer_cast<GlfwMonitor>(desc.monitor)->native();
			if (desc.parent)
				glfwParentWindow = std::static_pointer_cast<GlfwWindow>(desc.parent)->native();
	
			m_window = m_instance->glfwCreateWindow(desc.videoMod.widith, desc.videoMod.height, to_utf8(m_title).c_str(), glfwMonitor, glfwParentWindow);
			geAssert(m_window);
		}

		virtual void maximaze() noexcept override
		{
			m_instance->glfwMaximizeWindow(m_window);
		}

		virtual void restore() noexcept override
		{
			m_instance->glfwRestoreWindow(m_window);
		}

		virtual void setLocation(int32 x, int32 y) override
		{
			m_instance->glfwSetWindowPos(m_window, x, y);
		}

		virtual void setSize(uint32 w, uint32 h)  override
		{
			m_instance->glfwSetWindowSize(m_window, w, h);
		}

		virtual void getSize(uint32& w, uint32& h) const noexcept override
		{
			int x, y;
			m_instance->glfwGetWindowSize(m_window, &x, &y);
			w = x;
			h = y;
		}

		virtual void getLocation(int32& x, int32& y) const noexcept  override
		{
			m_instance->glfwGetWindowPos(m_window, &x, &y);
		}

		virtual String getTitle() const override
		{
			return m_title;
		}

		virtual void setTitle(const String& title) override
		{
			m_title = title;
			m_instance->glfwSetWindowTitle(m_window, to_utf8(m_title).c_str());
		}

		virtual bool isClosed() const noexcept override
		{	
			return m_instance->glfwWindowShouldClose(m_window);
		}

		virtual void setFullscreen(const Ptr<Monitor>& monitor, int32 x, int32 y, const VideoMod& mod) override
		{
			glfw::GLFWmonitor* glfwMonitor = nullptr;
			if (monitor) 
			{
				glfwMonitor = std::static_pointer_cast<GlfwMonitor>(monitor)->native();
				auto findedMonitor = m_instance->monitors().find(glfwMonitor);			
				geAssert(findedMonitor != m_instance->monitors().end());
				geAssert(findedMonitor->second == monitor);
			}
			m_instance->glfwSetWindowMonitor(m_window, glfwMonitor, x, y, mod.widith, mod.height, mod.refreshRate);
		}

		virtual void setWindowed(int32 x, int32 y, uint32 w, uint32 h) override
		{
			m_instance->glfwSetWindowMonitor(m_window, nullptr, x, y, w, h, -1);
		}

	};

	GlfwWindowManager::~GlfwWindowManager()
	{
		glfwTerminate();
	}

	GlfwWindowManager::GlfwWindowManager()
	{
#		ifdef _DEBUG
			String buildType = u"Debug";
#		else
			String buildType = u"Release";
#		endif

		String libraryName = format("GETP{0}_{1}", (sizeof(void*) == 4) ? "32" : "64", buildType);
		m_glfwLibrary = snew<Library>(libraryName);
		if (!m_glfwLibrary->isLoad())
			geAssertFalse("Can not load GETP@.dll triple.");
#			define LinkProc(x) x = m_glfwLibrary->getProcessAddress<decltype(x)>(u ## # x); geAssert(x)

		LinkProc(glfwInit);
		LinkProc(glfwTerminate);
		LinkProc(glfwCreateWindow);
		LinkProc(glfwDestroyWindow);
		LinkProc(glfwWindowShouldClose);
		LinkProc(glfwPollEvents);
		LinkProc(glfwMaximizeWindow);
		LinkProc(glfwRestoreWindow);
		LinkProc(glfwGetWindowSize);
		LinkProc(glfwSetWindowSize);
		LinkProc(glfwGetWindowPos);
		LinkProc(glfwSetWindowPos);
		LinkProc(glfwSetWindowTitle);

		LinkProc(glfwGetMonitorWorkarea);
		LinkProc(glfwGetMonitorPhysicalSize);
		LinkProc(glfwGetMonitorContentScale);
		LinkProc(glfwGetMonitorName);
		LinkProc(glfwGetVideoModes);
		LinkProc(glfwGetMonitorPos);
		LinkProc(glfwGetPrimaryMonitor);
		LinkProc(glfwGetMonitors);
		LinkProc(glfwGetVideoMode);
		LinkProc(glfwGetWin32Window);

		glfwInit();
	}

	Ptr<Monitor> GlfwWindowManager::primaryMonitor()
	{
		glfw::GLFWmonitor* nativeMonitor = glfwGetPrimaryMonitor();
		auto monitor = m_monitors.find(nativeMonitor);
		if (monitor == m_monitors.end())
		{
			auto resultMonitor = std::static_pointer_cast<Monitor>(snew<GlfwMonitor>(nativeMonitor, this));
			m_monitors[nativeMonitor] = resultMonitor;
			return resultMonitor;
		}
		return monitor->second;
	}

	Vector<Ptr<Monitor>> GlfwWindowManager::allMonitors()
	{
		Vector<Ptr<Monitor>> resultMonitors;
		int count;
		glfw::GLFWmonitor** monitors = glfwGetMonitors(&count);
		std::map<glfw::GLFWmonitor*, Ptr<Monitor>> newMonitors;
		for (int i = 0; i < count; i++)
		{
			auto monitor = m_monitors.find(monitors[i]);
			if (monitor == m_monitors.end())
				newMonitors[monitors[i]] = std::static_pointer_cast<Monitor>(snew<GlfwMonitor>(monitors[i], this));
			else
				newMonitors[monitors[i]] = monitor->second;		
		}
		m_monitors = std::move(newMonitors);
		for (auto& x : m_monitors)
			resultMonitors.push_back(x.second);
		return std::move(resultMonitors);

	}

	Ptr<Window> GlfwWindowManager::create(const WINDOW_DESC& desc)
	{
		return snew<GlfwWindow>(desc, this);;
	}

	void GlfwWindowManager::dispatchMessages()
	{
		glfwPollEvents();
	}
}

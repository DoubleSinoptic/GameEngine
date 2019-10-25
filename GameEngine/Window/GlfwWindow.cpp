#include "Window.h"
#include "Core/StringFormat.h"
#include "Core/Library.h"

typedef struct GLFWmonitor GLFWmonitor;
typedef struct GLFWwindow GLFWwindow;

namespace ge 
{
	class GlfwInstance 
	{
		Ptr<Library> m_glfwLibrary;
	public:

		int			(*glfwInit)(void);
		void		(*glfwTerminate)(void);
		GLFWwindow* (*glfwCreateWindow)(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
		void		(*glfwDestroyWindow)(GLFWwindow* window);
		int			(*glfwWindowShouldClose)(GLFWwindow* window);
		void		(*glfwPollEvents)(void);
	    void		(*glfwMaximizeWindow)(GLFWwindow* window);
		void		(*glfwRestoreWindow)(GLFWwindow* window);
		void		(*glfwGetWindowSize)(GLFWwindow* window, int* width, int* height);
		void		(*glfwSetWindowSize)(GLFWwindow* window, int width, int height);
		void		(*glfwGetWindowPos)(GLFWwindow* window, int* xpos, int* ypos);
		void		(*glfwSetWindowPos)(GLFWwindow* window, int xpos, int ypos);
		void		(*glfwSetWindowTitle)(GLFWwindow* window, const char* title);

		GlfwInstance() 
		{
#			ifdef _DEBUG
				String buildType = u"Debug";
#			else
				String buildType = u"Release";
#			endif

			String libraryName = format("glfw3_{0}_{1}", (sizeof(void*) == 4) ? "32" : "64", buildType);
			m_glfwLibrary = snew<Library>(libraryName);
			if (!m_glfwLibrary->isLoad())
				geAssertFalse("Can not load glfw3@.dll triple.");
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
			glfwInit();
		}

		~GlfwInstance() 
		{
			glfwTerminate();
		}
	};

	Ptr<GlfwInstance> glfwGlobalInstance;

	class GlfwWindow : public Window
	{
		String				m_title;
		Ptr<GlfwInstance>	m_instance;
		GLFWwindow*			m_window;
	public:
		~GlfwWindow() 
		{	
			m_instance->glfwDestroyWindow(m_window);
			m_instance = nullptr;
			if (glfwGlobalInstance.use_count() == 1)
				glfwGlobalInstance = nullptr;
		}

		GlfwWindow(uint32 w, uint32 h, const String& title) :
			m_instance(glfwGlobalInstance),
			m_window(nullptr),
			m_title(title)
		{
			if (!glfwGlobalInstance)
			{
				glfwGlobalInstance = snew <GlfwInstance>();
				m_instance = glfwGlobalInstance;
			}
				
			::std::string glfwTitle(title.begin(), title.end());
			m_window = m_instance->glfwCreateWindow(w, h, glfwTitle.c_str(), nullptr, nullptr);
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

		virtual void getSize(uint32& w, uint32 h) const noexcept override
		{
			int x, y;
			m_instance->glfwGetWindowSize(m_window, &x, &y);
			w = x;
			h = y;
		}

		virtual void getLocation(int32& x, int32 y) const noexcept  override
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
			::std::string glfwTitle(title.begin(), title.end());
			m_instance->glfwSetWindowTitle(m_window, glfwTitle.c_str());
		}

		virtual void setFullsreen(bool value) override
		{

		}

		virtual void dispatchMessages() override
		{
			m_instance->glfwPollEvents();
		}

		virtual bool isClosed() const noexcept override
		{	
			return m_instance->glfwWindowShouldClose(m_window);
		}
	};

	Ptr<Window> createWin32GlfwWindow(uint32 w, uint32 h, const String& name) 
	{
		return snew<GlfwWindow>(w, h, name);
	}
}

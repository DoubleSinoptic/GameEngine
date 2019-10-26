#pragma once
#ifndef __GLFWWINDOWMANAGER_H__
#define __GLFWWINDOWMANAGER_H__

#include "Core/Core.h"
#include "Core/Library.h"
#include "WindowManager.h"
#include "Window.h"
#include <map>

namespace ge
{
	namespace glfw
	{
		typedef struct GLFWmonitor GLFWmonitor;
		typedef struct GLFWwindow GLFWwindow;
		typedef struct GLFWvidmode
		{
			int width;
			int height;
			int redBits;
			int greenBits;
			int blueBits;
			int refreshRate;
		} GLFWvidmode;
	}

	class GlfwWindowManager : public WindowManager 
	{
		Ptr<Library> m_glfwLibrary;
		std::map<glfw::GLFWmonitor*, Ptr<Monitor>> m_monitors;
	public:
		std::map<glfw::GLFWmonitor*, Ptr<Monitor>>& monitors() 
		{
			return m_monitors;
		}

		const std::map<glfw::GLFWmonitor*, Ptr<Monitor>>& monitors() const
		{
			return m_monitors;
		}

		int					(*glfwInit)(void);
		void				(*glfwTerminate)(void);
		glfw::GLFWwindow*   (*glfwCreateWindow)(int width, int height, const char* title, glfw::GLFWmonitor* monitor, glfw::GLFWwindow* share);
		void				(*glfwDestroyWindow)(glfw::GLFWwindow* window);
		int					(*glfwWindowShouldClose)(glfw::GLFWwindow* window);
		void				(*glfwPollEvents)(void);
		void				(*glfwMaximizeWindow)(glfw::GLFWwindow* window);
		void				(*glfwRestoreWindow)(glfw::GLFWwindow* window);
		void				(*glfwGetWindowSize)(glfw::GLFWwindow* window, int* width, int* height);
		void				(*glfwSetWindowSize)(glfw::GLFWwindow* window, int width, int height);
		void				(*glfwGetWindowPos)(glfw::GLFWwindow* window, int* xpos, int* ypos);
		void				(*glfwSetWindowPos)(glfw::GLFWwindow* window, int xpos, int ypos);
		void				(*glfwSetWindowTitle)(glfw::GLFWwindow* window, const char* title);


		void				(*glfwGetMonitorWorkarea)(glfw::GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height);
		void				(*glfwGetMonitorPhysicalSize)(glfw::GLFWmonitor* monitor, int* widthMM, int* heightMM);
		void				(*glfwGetMonitorContentScale)(glfw::GLFWmonitor* monitor, float* xscale, float* yscale);
		const char*			(*glfwGetMonitorName)(glfw::GLFWmonitor* monitor);
		const glfw::GLFWvidmode* (*glfwGetVideoModes)(glfw::GLFWmonitor* monitor, int* count);
		void (*glfwGetMonitorPos)(glfw::GLFWmonitor* monitor, int* xpos, int* ypos);
		glfw::GLFWmonitor*	(*glfwGetPrimaryMonitor)(void);
		glfw::GLFWmonitor** (*glfwGetMonitors)(int* count);
		void				(*glfwSetWindowMonitor)(glfw::GLFWwindow* window, glfw::GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

		~GlfwWindowManager();
		GlfwWindowManager();

		virtual Ptr<Monitor>			primaryMonitor() override;
		virtual Vector<Ptr<Monitor>>	allMonitors() override;
		virtual Ptr<Window>				create(const WINDOW_DESC& desc) override;
		virtual void					dispatchMessages() override;
	};
}

#endif
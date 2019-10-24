#include "Window.h"

namespace ge 
{
	Ptr<Window> createWin32GlfwWindow(uint32 w, uint32 h, const String& name);

	Ptr<Window> Window::create(uint32 w, uint32 h, const String& title)
	{
		return createWin32GlfwWindow(w, h, title);
	}
}


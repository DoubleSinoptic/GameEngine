#include "Window.h"
namespace ge 
{
	RPtr<ResourceObject> Window::getResource(const String& resource)
	{
		auto iter = m_resources.find(resource);
		if (iter == m_resources.end())
			return nullptr;
		return iter->second;
	}

	void Window::setResource(const String& name, RPtr<ResourceObject> resource)
	{
		m_resources.emplace(name, resource);
	}
}

#include "VulkanTexture2D.h"
namespace ge 
{
	VulkanTexture2D::VulkanTexture2D(TEXTURE2D_DESC& desc, VulkanGpuContext* instance) :
		m_instance(instance),
		Texture2D(desc)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

}

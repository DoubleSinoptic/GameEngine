#pragma once
#ifndef __VULKANTEXTURE2D_H__
#define __VULKANTEXTURE2D_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "Rendering/Texture2D.h"

namespace ge
{
	class VulkanTexture2D : public Texture2D
	{
		VulkanGpuContext* m_instance;
	public:
		VulkanTexture2D(TEXTURE2D_DESC& desc, VulkanGpuContext* instance);
		~VulkanTexture2D();
	};
}

#endif
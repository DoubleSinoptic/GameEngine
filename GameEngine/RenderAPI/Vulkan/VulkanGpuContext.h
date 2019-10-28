#pragma once
#ifndef __VULKANGPUCONTEXT_H__
#define __VULKANGPUCONTEXT_H__

#include "Core/Core.h"
#include "Core/Any.h"
#include "../GpuContext.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <map>

#define CHECK_VULKAN(x) {if ((x) != VK_SUCCESS) { geAssertFalse(# x); }}	

namespace ge
{
	class VulkanGpuContext : public GpuContext
	{
		std::map<PixelFormat, VkFormat> m_toVkFormats;
		std::map<VkFormat, PixelFormat> m_toGeFormats;
	public:
		VkDevice			device;
		VkPhysicalDevice	physicalDevice;
		VmaAllocator		allocator;
		VulkanGpuContext();
		~VulkanGpuContext();

		VkFormat	getVkFormat(PixelFormat fmt) const;
		PixelFormat getGeFormat(VkFormat fmt) const;
	};
}

#endif
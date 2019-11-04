#pragma once
#ifndef __VULKANGPUCONTEXT_H__
#define __VULKANGPUCONTEXT_H__

#include "Core/Core.h"
#include "Core/Any.h"
#include "../GpuContext.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <map>
#include <unordered_map>
#include <set>

#define CHECK_VULKAN(x) {if ((x) != VK_SUCCESS) { geAssertFalse(# x); }}	

namespace ge
{
	class VulkanCommandBuffer;
	struct ImageBarrierTriple
	{
		VkImageLayout		 layout;
		VkAccessFlags		 access;
		VkPipelineStageFlags stage;
	};
	class VulkanGpuContext : public GpuContext
	{
		std::unordered_map<PixelFormat, VkFormat>				m_toVkFormats;
		std::unordered_map<VkFormat, PixelFormat>				m_toGeFormats;

		struct UniformDescHash
		{
			usize operator()(const UNIFORM_DESC& value) const
			{
				return byteArrayHash((const byte*)& value, sizeof(value));
			}
		};
		struct UniformDescEqual
		{
			usize operator()(const UNIFORM_DESC& a, const UNIFORM_DESC& b) const
			{
				return !memcmp((const void*)& a, (const void*)& b, sizeof(UNIFORM_DESC));
			}
		};


		std::unordered_map<UNIFORM_DESC, VkDescriptorSetLayout, UniformDescHash, UniformDescEqual> m_descriptorSetLayouts;
	public:
		VkDevice			device;
		VkPhysicalDevice	physicalDevice;
		VmaAllocator		allocator;

		std::set<RPtr<VulkanCommandBuffer>>	 activeCommandBuffers;

		VulkanGpuContext();
		~VulkanGpuContext();

		int32 getQueueFamailyIndex(QueueType type) const;

		VkFormat	getVkFormat(PixelFormat fmt) const;
		PixelFormat getGeFormat(VkFormat fmt) const;

		VkDescriptorSetLayout getLayoutFromDesc(const UNIFORM_DESC& a);

	};
}

#endif
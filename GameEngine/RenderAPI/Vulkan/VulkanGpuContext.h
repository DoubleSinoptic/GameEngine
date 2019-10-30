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

		struct ImageBarrierTriple 
		{
			VkImageLayout		 layout;
			VkAccessFlags		 aceess;
			VkPipelineStageFlags stage;		
		};

		void switchLayout(
			VkImage image, VkImageAspectFlags aspectFlags,
			uint32 mip, uint32 layer, uint32 mN, uint32 lN,
			ImageBarrierTriple oldState,
			ImageBarrierTriple newState)
		{
			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldState.layout;
			barrier.newLayout = newState.layout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = aspectFlags;
			barrier.subresourceRange.baseMipLevel = mip;
			barrier.subresourceRange.levelCount = mN;
			barrier.subresourceRange.baseArrayLayer = layer;
			barrier.subresourceRange.layerCount = lN;

			barrier.srcAccessMask = oldState.aceess;
			barrier.dstAccessMask = newState.aceess;

			/*
				vkCmdPipelineBarrier(*cmd,
				oldUseStage == VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT : oldUseStage,
				useStage, 0,
				0, 0, 0, 0, 1, &barrier);
			
			*/
		
		}
	};
}

#endif
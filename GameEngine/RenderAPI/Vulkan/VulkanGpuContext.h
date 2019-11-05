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

		VulkanGpuContext();
		~VulkanGpuContext();

		int32 getQueueFamailyIndex(QueueType type) const;

		VkFormat	getVkFormat(PixelFormat fmt) const;
		PixelFormat getGeFormat(VkFormat fmt) const;

		VkDescriptorSetLayout getLayoutFromDesc(const UNIFORM_DESC& a);

		virtual void submit(CommandBuffer* cmdBuffer) override;

		virtual Uniform* createUniform(const UNIFORM_DESC& desc) override;

		virtual Texture2D* createTexture2D(const TEXTURE2D_DESC& desc) override;

		virtual Buffer* createBuffer(const BUFFER_DESC& desc) override;

		virtual Sampler* createSampler(const SAMPLER_DESC& desc) override;

		virtual CommandBuffer* createCommandBuffer(const COMMAND_BUFFER_DESC& desc) override;

		virtual Pipeline* createPipeline(const PIPELINE_DESC& desc) override;

		virtual Framebuffer* createFramebuffer(const FRAMEBUFFER_DESC& desc) override;

		virtual usize allignUniform(usize length) const override;

	};
}

#endif
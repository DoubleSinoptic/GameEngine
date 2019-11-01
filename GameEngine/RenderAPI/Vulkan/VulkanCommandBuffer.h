#pragma once
#ifndef __VULKANCOMMANDBUFFER_H__
#define __VULKANCOMMANDBUFFER_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "VulkanGpuContext.h"
#include "Core/PoolAllocator.h"

namespace ge
{
	class VulkanCommandBuffer;

	class VulkanCommandPool
	{
		VkCommandPool				 m_cmdPool;
		VulkanGpuContext*			 m_instance;
		std::vector<VkCommandBuffer> m_buffers;

	public:
		void freeBuffer(VkCommandBuffer buffer)
		{
			m_buffers.push_back(buffer);
			vkResetCommandBuffer(buffer, 0);
		}

		VulkanCommandBuffer* allocate() 
		{
			return PoolAllocator::allocate<VulkanCommandBuffer>(this, (VkCommandBuffer)nullptr);	
		}
	};

	class VulkanCommandBuffer : public ResourceObject
	{
		VulkanGpuContext* m_instance;
		VkCommandBuffer							m_buffer;
		VulkanCommandPool*						m_pool;
		std::vector<std::function<void(void)>>	m_registredReleases;
		VulkanCommandBufferState				m_state;
	public:
		virtual void release() const noexcept override;

		VulkanCommandBuffer(VulkanCommandPool* m_pool, VkCommandBuffer buffer);

		void update();

		void setState(VulkanCommandBufferState state);

		VulkanCommandBufferState state() const noexcept;

		void registerRelease(const std::function<void(void)>& releaseFunction);
	};
}

#endif
#pragma once
#ifndef __VULKANSYNC_H__
#define __VULKANSYNC_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"

namespace ge
{
	class VulkanSemaphore : public GpuResource
	{
		VkSemaphore m_semaphore;
	public:
		VulkanSemaphore(VulkanGpuContext* context);
		~VulkanSemaphore();

		constexpr VkSemaphore vulkanHandle() const noexcept 
		{
			return m_semaphore;
		}
	};

	class VulkanFence : public GpuResource
	{
		VkFence m_fence;
	public:
		VulkanFence(VulkanGpuContext* context, bool signaled);
		~VulkanFence();

		bool wait(uint64 nan);
		void reset();

		constexpr VkFence vulkanHandle() const noexcept
		{
			return m_fence;
		}
	};
}

#endif
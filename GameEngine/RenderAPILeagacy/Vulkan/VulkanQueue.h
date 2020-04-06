#pragma once
#ifndef __VULKANQUEUE_H__
#define __VULKANQUEUE_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "VulkanCommandBuffer.h"

namespace ge
{
	class VulkanQueue
	{
		std::map<uint32, Ptr<VulkanCommandPool>>	m_pools;
		std::mutex									m_mutex;
		VulkanGpuContext*							m_context;
		VkQueue										m_queue;
	public:
		VulkanQueue(VulkanGpuContext* ctx, uint32 ) :
			m_context(ctx)
		{
		
		}

		constexpr VkQueue vulkanHandle() const noexcept 
		{
			return m_queue;
		}

		Ptr<VulkanCommandPool> getPool(uint32 poolIndex)
		{
			auto iter = m_pools.find(poolIndex);
			if (iter == m_pools.end())
			{
				Ptr<VulkanCommandPool> pool = snew<VulkanCommandPool>(m_context, );
				m_pools.emplace(poolIndex, pool);
				return pool;
			}
			return iter->second;
		}

		~VulkanQueue()
		{}
	};

}

#endif
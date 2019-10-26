#pragma once
#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "Rendering/Buffer.h"

namespace ge
{
	class VulkanBuffer : public Buffer
	{
		VulkanGpuContext*		m_instance;
		VkBuffer				m_buffer;
		VkDeviceMemory			m_memory;
		VkDeviceSize			m_memoryOffset;
		VmaAllocation			m_allocation;
		VkDeviceSize			m_size;
		bool					m_hasMapped;
		VulkanBuffer*			m_mappedBuffer;
		VkDeviceSize			m_mappedOffset;
		VkDeviceSize			m_mappedSize;
		bool					m_directMod;
	public:
		VulkanBuffer(const BUFFER_DESC& desc, bool transferSrc, VulkanGpuContext* instance);
		~VulkanBuffer();

		virtual void* map(usize offset, usize length, AccessFlags flags) override;
		virtual void unmap() override;
	};
}

#endif
#include "VulkanCommandBuffer.h"
#include "VulkanGpuContext.h"

namespace ge 
{
	void VulkanCommandBuffer::release() const noexcept
	{
		if (deincrement())
		{
			m_pool->freeBuffer(m_buffer);
			PoolAllocator::free(this);
		}
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandPool* m_pool, VkCommandBuffer buffer) :
		m_state(VCBS_RECORDERING),
		m_buffer(buffer),
		m_pool(m_pool)
	{
		m_instance->activeCommandBuffers.insert(this);
		VkCommandBufferBeginInfo begin = {};
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(m_buffer, &begin);
	}

	void VulkanCommandBuffer::setState(VulkanCommandBufferState state)
	{
		switch (state)
		{
		case ge::VCBS_RECORDERING:
			geAssertFalse("Invalid buffer state.");
			break;
		case ge::VCBS_IN_FLIGHT:
			vkEndCommandBuffer(m_buffer);
			break;
		case ge::VCBS_FINISHED:
			for (auto& x : m_registredReleases)
				x();
			m_registredReleases.clear();
			break;
		default:
			geAssertFalse("Invalid buffer usage.");
			break;
		}	
		m_state = state;
	}

	VulkanCommandBufferState VulkanCommandBuffer::state() const noexcept
	{
		return m_state;
	}

	void VulkanCommandBuffer::registerRelease(const std::function<void(void)>& releaseFunction)
	{
		m_registredReleases.push_back(releaseFunction);
	}
}

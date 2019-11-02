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

	VulkanCommandBuffer::VulkanCommandBuffer(const COMMAND_BUFFER_DESC& desc, VulkanGpuContext* context, VulkanCommandPool* m_pool, VkCommandBuffer buffer) :
		m_buffer(buffer),
		m_pool(m_pool),
		m_instance(context),
		CommandBuffer(desc, context)
	{
		m_instance->activeCommandBuffers.insert(this);
		VkCommandBufferBeginInfo begin = {};
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(m_buffer, &begin);
	}

	bool VulkanCommandBuffer::isFinished()
	{
		return false;
	}

	void VulkanCommandBuffer::copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart)
	{
	}

	void VulkanCommandBuffer::copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg)
	{
	}

	void VulkanCommandBuffer::copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter)
	{
	}

	void VulkanCommandBuffer::drawIndexed(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces)
	{
	}

	void VulkanCommandBuffer::draw(uint32 firstVerteces, uint32 numVerteces)
	{
	}

	void VulkanCommandBuffer::drawIndexedInstanced(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces, uint32 firstInstnace, uint32 instancesNum)
	{
	}

	void VulkanCommandBuffer::setClearColors(const CLEAR_COLOR* clearColors, usize num)
	{
	}

	void VulkanCommandBuffer::setPipeline(Pipeline* pipeline)
	{
	}

	void VulkanCommandBuffer::setFrameBuffer(Framebuffer* framebuffer)
	{
	}

	void VulkanCommandBuffer::setUniforms(Uniform* const* uniform, uint32 start, uint32 num)
	{
	}

	void VulkanCommandBuffer::setUniform(Uniform* uniform, uint32 start = 0)
	{
	}

	void VulkanCommandBuffer::execute(CommandBuffer* secondryCommandBuffer)
	{
	}


}

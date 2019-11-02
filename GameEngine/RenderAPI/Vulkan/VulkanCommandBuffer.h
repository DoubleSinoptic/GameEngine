#pragma once
#ifndef __VULKANCOMMANDBUFFER_H__
#define __VULKANCOMMANDBUFFER_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "VulkanGpuContext.h"
#include "../CommandBuffer.h"
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

	class VulkanCommandBuffer : public CommandBuffer
	{
		VulkanGpuContext*						m_instance;
		VkCommandBuffer							m_buffer;
		VulkanCommandPool*						m_pool;
	public:
		virtual void release() const noexcept override;

		VulkanCommandBuffer(const COMMAND_BUFFER_DESC& desc, VulkanGpuContext* context, VulkanCommandPool* m_pool, VkCommandBuffer buffer);


		// Унаследовано через CommandBuffer
		virtual bool isFinished() override;

		virtual void copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart) override;

		virtual void copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg) override;

		virtual void copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter) override;

		virtual void drawIndexed(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces) override;

		virtual void draw(uint32 firstVerteces, uint32 numVerteces) override;

		virtual void drawIndexedInstanced(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces, uint32 firstInstnace, uint32 instancesNum) override;

		virtual void setClearColors(const CLEAR_COLOR* clearColors, usize num) override;

		virtual void setPipeline(Pipeline* pipeline) override;

		virtual void setFrameBuffer(Framebuffer* framebuffer) override;

		virtual void setUniforms(Uniform* const* uniform, uint32 start, uint32 num) override;

		virtual void setUniform(Uniform* uniform, uint32 start = 0) override;

		virtual void execute(CommandBuffer* secondryCommandBuffer) override;

	};
}

#endif
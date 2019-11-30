#pragma once
#ifndef __VULKANCOMMANDBUFFER_H__
#define __VULKANCOMMANDBUFFER_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "VulkanGpuContext.h"
#include "../CommandBuffer.h"
#include "Core/PoolAllocator.h"
#include "VulkanSync.h"

namespace ge
{
	class VulkanTexture2D;
	class VulkanCommandBuffer;
	class VulkanFramebuffer;
	class VulkanCommandPool
	{
		VkCommandPool				 m_cmdPool;
		VulkanGpuContext*			 m_instance;
		std::vector<VkCommandBuffer> m_buffers;
		COMMAND_BUFFER_DESC			 m_desc;

	public:
		VulkanCommandPool(VulkanGpuContext* instance, const COMMAND_BUFFER_DESC& type);

		~VulkanCommandPool();

		void freeBuffer(VkCommandBuffer buffer);

		VulkanCommandBuffer* allocate();
	};

	class VulkanCommandBuffer : public CommandBuffer
	{
		VulkanGpuContext*						m_instance;
		VkCommandBuffer							m_buffer;
		VulkanCommandPool*						m_pool;
		VulkanFramebuffer*						m_currentFramebuffer;
		VkPipelineLayout						m_currentPipelineLayout;
		CLEAR_COLOR								m_clearColors[16];
		usize								    m_clearColorsNum;
		VkFence									m_finishFence;
		Vector<RPtr<VulkanSemaphore>>			m_waitSemaphores;
		Vector<RPtr<VulkanSemaphore>>			m_signalSemaphores;
	public:
		void registerWait(VulkanSemaphore* semaphore);
		void registerSignal(VulkanSemaphore* semaphore);

		constexpr VkCommandBuffer vulkanCb() const noexcept
		{
			return m_buffer;
		}

		VulkanCommandBuffer(const COMMAND_BUFFER_DESC& desc, VulkanGpuContext* context, VulkanCommandPool* m_pool, VkCommandBuffer buffer);
		~VulkanCommandBuffer();

		VkFence prepareForSubmit();

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

		virtual void setIndecesBuffer(Buffer* buffer, uint32 offset, bool x32bit) override;

		virtual void setVertexBuffer(Buffer* vertexBuffer, uint32 first, uint32 offset) override;

		void switchLayout(
			VkImage image, VkImageAspectFlags aspectFlags,
			uint32 mip, uint32 layer, uint32 mN, uint32 lN,
			ImageBarrierTriple oldState,
			ImageBarrierTriple newState
		);
	};
}

#endif
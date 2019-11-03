#include "VulkanCommandBuffer.h"
#include "VulkanGpuContext.h"
#include "VulkanUniform.h"
#include "VulkanBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanFrameBuffer.h"
#include "VulkanTexture2D.h"
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
		CommandBuffer(desc, context),
		m_clearColorsNum(0)
	{
		m_instance->activeCommandBuffers.insert(this);
		VkCommandBufferBeginInfo begin = {};
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(m_buffer, &begin);
	}

	void VulkanCommandBuffer::endRecord()
	{
	}



	bool VulkanCommandBuffer::isFinished()
	{
		return false;
	}

	void VulkanCommandBuffer::copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart)
	{
		VulkanBuffer* vdst = static_cast<VulkanBuffer*>(dst);
		VulkanBuffer* vsrc = static_cast<VulkanBuffer*>(src);
		VkBufferCopy cpy = {};
		cpy.dstOffset = dstStart;
		cpy.srcOffset = srcStart;
		cpy.size = size;
		vkCmdCopyBuffer(m_buffer, vsrc->vulkanHandle(), vdst->vulkanHandle(), 1, &cpy);
		trackResource(dst);
		trackResource(src);
	}

	void VulkanCommandBuffer::copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg)
	{
		/*VulkanTexture* tx = static_cast<VulkanTexture*>(dst);
		VulkanBuffer* bf = static_cast<VulkanBuffer*>(src);
		VkBufferImageCopy region = {};
		region.bufferOffset = srcStart;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = dstReg->mipLevel;
		region.imageSubresource.baseArrayLayer = dstReg->arrayLayer;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { SInt(dstReg->x), SInt(dstReg->y), 0 };
		region.imageExtent = {
			dstReg->width,
			dstReg->heght,
			1
		};

		tx->SwithLayout2C(this, tx->m_baseLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstReg->mipLevel, dstReg->arrayLayer);
		vkCmdCopyBufferToImage(m_commandBuffer, bf->m_buffer, tx->m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		tx->SwithLayout2C(this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, tx->m_baseLayout, dstReg->mipLevel, dstReg->arrayLayer);


		trackResource(dst);
		trackResource(src);*/
	}

	void VulkanCommandBuffer::copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter)
	{
		VulkanTexture2D* d = static_cast<VulkanTexture2D*>(dst);
		VulkanTexture2D* s = static_cast<VulkanTexture2D*>(src);

		VkImageBlit cpy = {};
		cpy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		cpy.dstSubresource.mipLevel = dstReg->mipLevel;
		cpy.dstSubresource.baseArrayLayer = dstReg->arrayLayer;
		cpy.dstSubresource.layerCount = 1;
		cpy.dstOffsets[0] = { int32(dstReg->x),  int32(dstReg->y), 0 };
		cpy.dstOffsets[1] = { int32(dstReg->x + dstReg->width),  int32(dstReg->y + dstReg->heght), 1 };

		cpy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		cpy.srcSubresource.mipLevel = srcReg->mipLevel;
		cpy.srcSubresource.baseArrayLayer = srcReg->arrayLayer;
		cpy.srcSubresource.layerCount = 1;
		cpy.srcOffsets[0] = { int32(srcReg->x),  int32(srcReg->y), 0 };
		cpy.srcOffsets[1] = { int32(srcReg->x + srcReg->width),  int32(srcReg->y + srcReg->heght), 1 };

		auto& commandBuffer = static_cast<VulkanCommandBuffer&>(m_instance->transferCb());

		commandBuffer.switchLayout(s->vulkanHandle(), s->aspectFlags(), srcReg->mipLevel, srcReg->arrayLayer, 1, 1, s->baseLayoutInfo(), 
			{ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT }
		);
		commandBuffer.switchLayout(d->vulkanHandle(), d->aspectFlags(), dstReg->mipLevel, dstReg->arrayLayer, 1, 1, d->baseLayoutInfo(),
			{ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT }
		);

		vkCmdBlitImage(m_buffer,
			s->vulkanHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			d->vulkanHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &cpy,
			filter == SF_LINEAR ? VK_FILTER_LINEAR : VK_FILTER_NEAREST);

		commandBuffer.switchLayout(s->vulkanHandle(), s->aspectFlags(), srcReg->mipLevel, srcReg->arrayLayer, 1, 1,
			{ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT }, s->baseLayoutInfo()
		);
		commandBuffer.switchLayout(d->vulkanHandle(), d->aspectFlags(), dstReg->mipLevel, dstReg->arrayLayer, 1, 1,
			{ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT }, d->baseLayoutInfo()
		);

		trackResource(dst);
		trackResource(src);
	}

	void VulkanCommandBuffer::drawIndexed(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces)
	{
		vkCmdDrawIndexed(m_buffer, numIndeces, 1, firstIndeces, firstVerteces, 0);
	}

	void VulkanCommandBuffer::draw(uint32 firstVerteces, uint32 numVerteces)
	{
		vkCmdDraw(m_buffer, numVerteces, 1, firstVerteces, 0);
	}

	void VulkanCommandBuffer::drawIndexedInstanced(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces, uint32 firstInstnace, uint32 instancesNum)
	{
		vkCmdDrawIndexed(m_buffer, numIndeces, instancesNum, firstIndeces, firstVerteces, firstInstnace);
	}

	void VulkanCommandBuffer::setClearColors(const CLEAR_COLOR* clearColors, usize num)
	{
		for (usize i = 0; i < num; i++)
			m_clearColors[i] = clearColors[i];	
		m_clearColorsNum = num;
	}

	void VulkanCommandBuffer::setPipeline(Pipeline* pipeline)
	{
		VulkanPipeline* vulkanPipeline = static_cast<VulkanPipeline*>(pipeline);
		m_currentPipelineLayout = vulkanPipeline->vulkanPipelineLayout();
		const auto& frmDesc = m_currentFramebuffer->getDesc();
		vkCmdBindPipeline(m_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			vulkanPipeline->get(m_currentFramebuffer->vulkanRenderPass(), frmDesc.width, frmDesc.heigth));

	}

	void VulkanCommandBuffer::setFrameBuffer(Framebuffer* framebuffer)
	{
	}

	void VulkanCommandBuffer::setUniforms(Uniform* const* uniform, uint32 start, uint32 num)
	{
		VkDescriptorSet bindedSets[16];
		for (uint32 i = 0; i < num; i++)
			bindedSets[i] = static_cast<VulkanUniform*>(uniform[i])->vulkanSet();
		vkCmdBindDescriptorSets(m_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout, start, num, bindedSets, 0, nullptr);
	}

	void VulkanCommandBuffer::setUniform(Uniform* uniform, uint32 start)
	{
		VkDescriptorSet set = static_cast<VulkanUniform*>(uniform)->vulkanSet();
		vkCmdBindDescriptorSets(m_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout, start, 1, &set, 0, nullptr);
	}

	void VulkanCommandBuffer::execute(CommandBuffer* secondryCommandBuffer)
	{
		VkCommandBuffer buffer = static_cast<VulkanCommandBuffer*>(secondryCommandBuffer)->vulkanCb();
		vkCmdExecuteCommands(m_buffer, 1, &buffer);
	}

	void VulkanCommandBuffer::setIndecesBuffer(Buffer* buffer, uint32 offset, bool x32bit)
	{
		VulkanBuffer* vbuff = static_cast<VulkanBuffer*>(buffer);
		vkCmdBindIndexBuffer(m_buffer, vbuff->vulkanHandle(), offset, x32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
	}

	void VulkanCommandBuffer::setVertexBuffer(Buffer* vertexBuffer, uint32 first, uint32 offset)
	{
		VkBuffer vbuff = static_cast<VulkanBuffer*>(vertexBuffer)->vulkanHandle();
		VkDeviceSize realOffset = offset;
		vkCmdBindVertexBuffers(m_buffer, first, 1, &vbuff, &realOffset);
	}
	
	void VulkanCommandBuffer::switchLayout(VkImage image, VkImageAspectFlags aspectFlags, uint32 mip, uint32 layer, uint32 mN, uint32 lN, ImageBarrierTriple oldState, ImageBarrierTriple newState)
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

		barrier.srcAccessMask = oldState.access;
		barrier.dstAccessMask = newState.access;

		vkCmdPipelineBarrier(m_buffer, oldState.stage, newState.stage, 0, 0, 0, 0, 0, 1, &barrier);
	}
}

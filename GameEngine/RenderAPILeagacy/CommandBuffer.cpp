#include "CommandBuffer.h"
#include "GpuContext.h"
namespace ge 
{
	CommandBuffer::~CommandBuffer()
	{
		for (auto& x : m_writeResources) {
			if (x == this)
				x->setWriteOwner(nullptr);
		}			
		m_writeResources.clear();
		m_resources.clear();
		gpuContext().m_commandBuffers.erase(this);
	}

	CommandBuffer::CommandBuffer(const COMMAND_BUFFER_DESC& desc, GpuContext* context) :
		m_desc(desc),
		GpuResource(context),
		m_enabledTraking(desc.usePredictedResourceTrack)
	{
		gpuContext().m_commandBuffers.insert(this);
	}

	const COMMAND_BUFFER_DESC& CommandBuffer::getDesc() const
	{
		return m_desc;
	}

	void CommandBuffer::trackResource(const RPtr<const GpuResource>& resource)
	{
		if (m_enabledTraking)
			m_resources.push_back(resource);
	}

	void CommandBuffer::ownAllResources()
	{
		for (auto& x : m_writeResources)
		{
			auto resourceOwner = x->getWriteOwner();
			if (resourceOwner != nullptr)
				resourceOwner->waitForFinish();
			x->setWriteOwner(this);
		}
	}

	void CommandBuffer::copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart)
	{
		copyBufferWR(dst, src, size, dstStart, srcStart);
		m_writeResources.emplace_back(dst);
		trackResource(src);
	}

	void CommandBuffer::copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg)
	{
		copyBufferToImageWR(dst, src, size, srcStart, dstReg);
		m_writeResources.emplace_back(dst);
		trackResource(src);
	}

	void CommandBuffer::copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter)
	{
		copyImageWR(dst, src, dstReg, srcReg, filter);
		m_writeResources.emplace_back(dst);
		trackResource(src);
	}

	void CommandBuffer::setFrameBuffer(Framebuffer* framebuffer) 
	{
		const auto& desc = framebuffer->getDesc();
		for (usize i = 0; i < desc.colorAttachmentsNum; i++)
			if (desc.colorAttachments[i].texture)
				m_writeResources.emplace_back(desc.colorAttachments[i].texture);		
		if (desc.depth.texture)
			m_writeResources.emplace_back(desc.depth.texture);
		if (desc.stencil.texture)
			m_writeResources.emplace_back(desc.stencil.texture);
		setFrameBufferWR(framebuffer);
		m_writeResources.emplace_back(framebuffer);
	}
}


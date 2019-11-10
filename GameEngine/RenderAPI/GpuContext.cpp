#include "GpuContext.h"
#include "CommandBuffer.h"
#include "../Rendering/RenderManager.h"
namespace ge 
{
	GpuContext& GpuContext::instance()
	{
		return rt::RenderManager::instance().gpuContext();
	}

	void GpuContext::releaseResource(const RPtr<const GpuResource>& resource)
	{
		resource->markAsReleased();
		for (auto commandBuffer : m_commandBuffers)
		{
			const GpuResource* commandBufferAsResource = static_cast<const GpuResource*>(commandBuffer);
			if (commandBufferAsResource != resource.get())
				commandBuffer->trackResource(resource);
		}
	}

	CommandBuffer& GpuContext::mainCb()
	{
		if (!m_mainCb)
		{
			COMMAND_BUFFER_DESC cmdBuffersDesc = {};
			cmdBuffersDesc.isSecondary = false;
			cmdBuffersDesc.poolIndex = 0;
			cmdBuffersDesc.queueType = QT_GRAPHICS;
			cmdBuffersDesc.usePredictedResourceTrack = true;
			m_mainCb = createCommandBuffer(cmdBuffersDesc);
		}
		return *m_mainCb;
	}

	CommandBuffer& GpuContext::transferCb()
	{
		if (!m_transferCb)
		{
			COMMAND_BUFFER_DESC cmdBuffersDesc = {};
			cmdBuffersDesc.isSecondary = false;
			cmdBuffersDesc.poolIndex = 0;
			cmdBuffersDesc.queueType = QT_TRANSFER;
			cmdBuffersDesc.usePredictedResourceTrack = false;
			m_transferCb = createCommandBuffer(cmdBuffersDesc);
		}
		return *m_transferCb;
	}

	void GpuContext::sumbit(CommandBufferTypeFlags types)
	{
		Vector<typename std::vector<RPtr<CommandBuffer>>::iterator> removedCommandBuffers;
		for (auto i = m_submitedBuffers.begin(); i != m_submitedBuffers.end(); i++)
			if ((*i)->isFinished())
				removedCommandBuffers.push_back(i);
		for (auto& x : removedCommandBuffers)
			m_submitedBuffers.erase(x);

		if (types & CBT_MAIN)
		{
			submit(m_mainCb);
			m_submitedBuffers.push_back(m_mainCb);
			m_mainCb = nullptr;
		}
		if (types & CBT_TRANSFER)
		{
			submit(m_transferCb);
			m_submitedBuffers.push_back(m_transferCb);
			m_transferCb = nullptr;
		}
	}

}

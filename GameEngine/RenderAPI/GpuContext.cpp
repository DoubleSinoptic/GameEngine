#include "GpuContext.h"
#include "CommandBuffer.h"
namespace ge 
{
	Ptr<GpuContext> currentGpuContext;

	void GpuContext::setCurrentGpuContext(Ptr<GpuContext> context)
	{
		currentGpuContext = context;
	}

	GpuContext& GpuContext::instance()
	{
		return *currentGpuContext;
	}

	void GpuContext::submit(CommandBuffer* cmdBuffer)
	{	
		geAssert(!cmdBuffer->isFinished());

		std::vector<typename std::set<RPtr<CommandBuffer>>::iterator> removeIterators;
		for (auto iter = m_activeCommandBuffers.begin(); iter != m_activeCommandBuffers.end(); iter++)
		{
			CommandBuffer* commandBuffer = *iter;
			if (commandBuffer->isFinished())
			{
				for (auto& x : commandBuffer->m_releaseCallbacks)
					x();
				commandBuffer->m_releaseCallbacks.clear();
				removeIterators.push_back(iter);		
			}
				
			if (commandBuffer->isRecordering() && commandBuffer->refCount() == 1)
				removeIterators.push_back(iter);
		}
		for (auto& x : removeIterators)
			m_activeCommandBuffers.erase(x);

		sumbmitCommandBuffer(cmdBuffer);
		cmdBuffer->isRecordering = false;
	}
	void GpuContext::registerRelease(const std::function<void(void)>& f, GpuAttachPlace flags)
	{
	}
	void GpuContext::registerResource(RPtr<ResourceObject> resource, GpuAttachPlace flags)
	{
	}
}

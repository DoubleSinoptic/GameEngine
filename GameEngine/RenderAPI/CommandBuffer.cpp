#include "CommandBuffer.h"
#include "GpuContext.h"
namespace ge 
{
	CommandBuffer::~CommandBuffer()
	{
		m_resources.clear();
		gpuContext().m_commandBuffers.erase(this);
	}

	CommandBuffer::CommandBuffer(const COMMAND_BUFFER_DESC& desc, bool track, GpuContext* context) :
		m_desc(desc),
		GpuResource(context),
		m_enabledTraking(track)
	{
		gpuContext().m_commandBuffers.insert(this);
	}

	const COMMAND_BUFFER_DESC& CommandBuffer::getDesc() const
	{
		return m_desc;
	}

	void CommandBuffer::trackResource(const RPtr<GpuResource>& resource)
	{
		if (m_enabledTraking)
			m_resources.push_back(resource);
	}

	
}


#include "CommandBuffer.h"
#include "GpuContext.h"
namespace ge 
{
	CommandBuffer::CommandBuffer(const COMMAND_BUFFER_DESC& desc) :
		m_desc(desc),
		m_isInRecrodering(true)
	{
		geAssert(desc.context);
		desc.context->m_activeCommandBuffers.insert(this);
	}

	const COMMAND_BUFFER_DESC& CommandBuffer::getDesc() const
	{
		return m_desc;
	}

	bool CommandBuffer::isRecordering() const
	{
		return m_isInRecrodering;
	}
}


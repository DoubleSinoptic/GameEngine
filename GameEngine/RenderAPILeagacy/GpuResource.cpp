#include "GpuResource.h"
#include "GpuContext.h"

namespace ge
{
	GpuResource::GpuResource(GpuContext* context) :
		m_isReleased(false),
		m_context(context),
		m_writeOwner(nullptr)
	{}

	void GpuResource::markAsReleased() const noexcept
	{
		m_isReleased = true;
	}

	CommandBuffer* GpuResource::getWriteOwner() const
	{
		return m_writeOwner;
	}

	void GpuResource::setWriteOwner(CommandBuffer* cmdBuffer)
	{
		m_writeOwner = cmdBuffer;
	}

	void GpuResource::release() const noexcept
	{
		if (deincrement())
		{
			if (!m_isReleased)
			{
				m_context->releaseResource(this);
			}
			else
				delete this;
		}
	}
}


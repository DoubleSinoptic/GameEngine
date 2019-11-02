#include "GpuResource.h"
#include "GpuContext.h"

namespace ge
{
	GpuResource::GpuResource(GpuContext* context) :
		m_isReleased(false),
		m_context(context)
	{}

	void GpuResource::markAsReleased() const noexcept
	{
		m_isReleased = true;
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


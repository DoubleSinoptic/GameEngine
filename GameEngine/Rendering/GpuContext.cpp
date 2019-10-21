#include "GpuContext.h"
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
}

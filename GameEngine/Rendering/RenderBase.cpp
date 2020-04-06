#include "RenderBase.h"

namespace ge 
{
	namespace rt
	{
		void InstacedInstance::bindBuffers(ICommandBuffer& context)
		{
		}
		usize InstacedInstance::instanceCount() const
		{
			return usize();
		}
		void InstacedInstance::updateTransform(const Matrix4& transform, usize index)
		{
		}
		usize InstacedInstance::addInstance(const Matrix4& mat)
		{
			return usize();
		}
		void InstacedInstance::removeInstance(usize index)
		{
		}

	}
}


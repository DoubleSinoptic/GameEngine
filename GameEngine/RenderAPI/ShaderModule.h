#pragma once
#ifndef __SHADERMODULE_H__
#define __SHADERMODULE_H__

#include "Core/Core.h"
#include "GpuResource.h"
#include "Uniform.h"

namespace ge
{
	struct SHADER_MODULE_DESC
	{
		Ptr<const Vector<byte>> code;
		PipelineStage	  stage;
		String			  entryPoint;
	};

	class ShaderModule : public GpuResource
	{
		const SHADER_MODULE_DESC m_desc;
	public:
		ShaderModule(const SHADER_MODULE_DESC& desc, GpuContext* context) :
			m_desc(desc),
			GpuResource(context)
		{}

		constexpr const SHADER_MODULE_DESC& getDesc() const noexcept
		{
			return m_desc;
		}
	};
}

#endif
#pragma once
#ifndef __SHADERMODULE_H__
#define __SHADERMODULE_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "Uniform.h"

namespace ge
{
	struct SHADER_MODULE_DESC
	{
		Ptr<Vector<byte>> code;
		PipelineStage	  stage;
		String			  entryPoint;
	};

	class ShaderModule : public ResourceObject 
	{
		const SHADER_MODULE_DESC m_desc;
	public:
		ShaderModule(const SHADER_MODULE_DESC& desc) :
			m_desc(desc)
		{}

		constexpr const SHADER_MODULE_DESC& getDesc() const noexcept
		{
			return m_desc;
		}
	};
}

#endif
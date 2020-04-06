#pragma once
#ifndef __VULKANSHADERMODULE_H__
#define __VULKANSHADERMODULE_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "../ShaderModule.h"

namespace ge
{
	class VulkanShaderModule : public ShaderModule
	{
		Utf8String m_entryPoint;
		VkPipelineShaderStageCreateInfo m_vertShaderStageInfo = {};
		VkShaderModule m_shaderModule;
	public:
		constexpr const VkPipelineShaderStageCreateInfo& stageCreationInfo() const noexcept 
		{
			return m_vertShaderStageInfo;
		}

		constexpr VkShaderModule vulanHandle() const noexcept 
		{
			return m_shaderModule;
		}

		VulkanShaderModule(const SHADER_MODULE_DESC& desc, VulkanGpuContext* instance);
		~VulkanShaderModule();
	};
}

#endif
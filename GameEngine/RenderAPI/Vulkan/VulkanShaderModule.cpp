#include "VulkanShaderModule.h"
namespace ge 
{
	VulkanShaderModule::VulkanShaderModule(const SHADER_MODULE_DESC& desc, VulkanGpuContext* instance) :
		ShaderModule(desc, instance)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc.code->size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(desc.code->data());

		CHECK_VULKAN(vkCreateShaderModule(gpuContextT<VulkanGpuContext>().device, &createInfo, nullptr, &m_shaderModule));

		m_vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		switch (desc.stage)
		{
		case PS_VERTEX:
			m_vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case PS_PIXEL:
			m_vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		case PS_NONE:
			geAssertFalse("Invalid code logic.");
			break;
		default:
			geAssertFalse("Invalid code logic.");
			break;
		}
		m_entryPoint = to_utf8(desc.entryPoint);
		m_vertShaderStageInfo.module = m_shaderModule;
		m_vertShaderStageInfo.pName = m_entryPoint.data();
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		if (m_shaderModule) {
			vkDestroyShaderModule(gpuContextT<VulkanGpuContext>().device, m_shaderModule, nullptr);
			m_shaderModule = VK_NULL_HANDLE;
		}
	}
}

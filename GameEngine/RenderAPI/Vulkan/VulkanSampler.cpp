#include "VulkanSampler.h"

namespace ge 
{
	VulkanSampler::VulkanSampler(const SAMPLER_DESC& desc, VulkanGpuContext* context) :
		m_instance(context),
		Sampler(desc)
	{
		static std::map<AddressMod, VkSamplerAddressMode> addresse =
		{
			{AddressMod::AM_MIRRORED_REPEAT, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT},
			{AddressMod::AM_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT},
			{AddressMod::AM_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER},
			{AddressMod::AM_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE},
			{AddressMod::AM_MIRRORED_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE},
			{AddressMod::AM_MIRRORED_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE}
		};

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = desc.mag == SF_LINEAR ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
		samplerInfo.minFilter = desc.min == SF_LINEAR ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
		samplerInfo.addressModeU = addresse[desc.addressModU];
		samplerInfo.addressModeV = addresse[desc.addressModV];
		samplerInfo.addressModeW = addresse[desc.addressModW];
		samplerInfo.anisotropyEnable = desc.anisotropy;
		samplerInfo.maxAnisotropy = desc.maxAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = desc.mip == SF_LINEAR ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.minLod = desc.minLod;
		samplerInfo.maxLod = desc.maxLod;
		samplerInfo.mipLodBias = 0;

		CHECK_VULKAN(vkCreateSampler(m_instance->device, &samplerInfo, nullptr, &m_sampler));
	}

	VulkanSampler::~VulkanSampler()
	{
		VulkanGpuContext* instance = m_instance;
		VkSampler	sampler = m_sampler;
		instance->registerRelease([=]() 
		{
			vkDestroySampler(instance->device, sampler, nullptr);		
		});
	}

}

#pragma once
#ifndef __VULKANSAMPLER_H__
#define __VULKANSAMPLER_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "../Sampler.h"

namespace ge
{
	class VulkanSampler : public Sampler
	{
		VkSampler		  m_sampler;
	public:
		VulkanSampler(const SAMPLER_DESC& desc, VulkanGpuContext* context);
		~VulkanSampler();
	};
}

#endif
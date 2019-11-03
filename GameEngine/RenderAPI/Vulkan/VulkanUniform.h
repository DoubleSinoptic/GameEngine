#pragma once
#ifndef __VULKANUNIFORM_H__
#define __VULKANUNIFORM_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "../Uniform.h"

namespace ge
{
	class VulkanUniform : public Uniform
	{
		VulkanGpuContext*	m_instance;
		VkDescriptorSet		m_set;
		VkDescriptorPool	m_pool;
	public:

		constexpr VkDescriptorSet vulkanSet() const noexcept 
		{
			return m_set;
		}
		constexpr VkDescriptorPool vulkanDescriptorPool() const noexcept
		{
			return m_pool;
		}

		VulkanUniform(const UNIFORM_DESC& desc, VulkanGpuContext* context);
		~VulkanUniform();
	};
}

#endif
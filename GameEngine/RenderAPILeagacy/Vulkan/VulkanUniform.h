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

		// Унаследовано через Uniform
		virtual void setSampler(Sampler* sampler, int32 slot) override;
		virtual void setTexture(Texture2D* texture, int32 slot) override;
		virtual void setBuffer(Buffer* buffer, int32 slot) override;
		virtual void setBuffer(Buffer* buffer, usize offset, usize length, int32 slot) override;
	};
}

#endif
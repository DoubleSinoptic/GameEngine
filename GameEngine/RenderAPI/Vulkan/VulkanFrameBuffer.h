#pragma once
#ifndef __VULKANFRAMEBUFFER_H__
#define __VULKANFRAMEBUFFER_H__

#include "Core/Core.h"
#include "../Framebuffer.h"
#include "VulkanGpuContext.h"

namespace ge
{
	class VulkanFramebuffer : public Framebuffer
	{
		VulkanGpuContext*	  m_instance;
		VkFramebuffer		  m_sampler;
	public:
		VulkanFramebuffer(const FRAMEBUFFER_DESC& desc, VulkanGpuContext* context);
		~VulkanFramebuffer();

		constexpr VkRenderPass vulkanRenderPass() const {
			return nullptr;
		}

	};
}

#endif
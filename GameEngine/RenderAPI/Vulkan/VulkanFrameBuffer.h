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
		VkFramebuffer		  m_framebuffer;
		VkRenderPass          m_renderPass;
	public:
		VulkanFramebuffer(const FRAMEBUFFER_DESC& desc, VulkanGpuContext* context);
		~VulkanFramebuffer();

		constexpr VkRenderPass vulkanRenderPass() const {
			return m_renderPass;
		}

		constexpr VkFramebuffer vulkanFramebuffer() const {
			return m_framebuffer;
		}
	};
}

#endif
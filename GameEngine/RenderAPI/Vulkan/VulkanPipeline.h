#pragma once
#ifndef __VULKANPIPELINE_H__
#define __VULKANPIPELINE_H__

#include "Core/Core.h"
#include "../Pipeline.h"
#include "VulkanGpuContext.h"
#include <unordered_map>

namespace ge
{
	class VulkanPipeline : public Pipeline
	{
		VulkanGpuContext*					m_instance;
		VkPipelineLayout					m_pipelineLayout;

		struct VkSizedRenderPass
		{
			VkRenderPass renerPass;
			uint32 w;
			uint32 h;
		};

		struct VkSizedRenderPassHash 
		{
			usize operator()(const VkSizedRenderPass& o) const 
			{
				return byteArrayHash((const byte*)&o, sizeof(VkSizedRenderPass));
			}
		};

		struct VkSizedRenderPassEqual
		{
			usize operator()(const VkSizedRenderPass& a, const VkSizedRenderPass& b) const
			{
				return !memcmp((const void*)&a, (const void*)&b, sizeof(VkSizedRenderPass));
			}
		};

		std::unordered_map<VkSizedRenderPass, VkPipeline, VkSizedRenderPassHash, VkSizedRenderPassEqual> m_pipelines;
	public:
		VulkanPipeline(const PIPELINE_DESC& desc, VulkanGpuContext* context);
		~VulkanPipeline();

		VkPipeline get(VkRenderPass renerPass, uint32 w, uint32 h);
	};
}

#endif
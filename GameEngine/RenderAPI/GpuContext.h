#pragma once
#ifndef __GPUCONTEXT_H__
#define __GPUCONTEXT_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

#include "Sampler.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "Uniform.h"
#include "Pipeline.h"
#include "Framebuffer.h"
#include "GpuContext.h"
#include <set>

namespace ge
{

	enum GpuAttachPlace
	{
		GAP_SUBMITED_BUFFER = 1 << 0,
		GAP_RECORDERED_BUFFER = 1 << 1,
		GAP_ALL = 0xFF
	};

	class CommandBuffer;
	class GpuContext
	{
		friend class CommandBuffer;
		std::set<RPtr<CommandBuffer>>	m_activeCommandBuffers;
	protected:
		virtual void sumbmitCommandBuffer(CommandBuffer* buffer) = 0;
	public:
		static void setCurrentGpuContext(Ptr<GpuContext> context);
		static GpuContext& instance();

		virtual ~GpuContext() = default;

		virtual Uniform* createUniform(const UNIFORM_DESC& desc) = 0;
		virtual Texture2D* createTexture2D(const TEXTURE2D_DESC& desc) = 0;
		virtual Buffer* createBuffer(const BUFFER_DESC& desc) = 0;
		virtual Sampler* createSampler(const SAMPLER_DESC& desc) = 0;

		virtual Pipeline* createPipeline(const PIPELINE_DESC& desc) = 0;
		virtual Framebuffer* createFramebuffer(const FRAMEBUFFER_DESC& desc) = 0;

		virtual usize allignUniform(usize length) const = 0;

		void registerRelease(const std::function<void(void)>& f, GpuAttachPlace flags = GAP_ALL);
		void registerResource(RPtr<ResourceObject> resource, GpuAttachPlace flags = GAP_ALL);

		virtual CommandBuffer* mainCb() = 0;
		virtual CommandBuffer* transferCb() = 0;
		virtual CommandBuffer* computeCb() = 0;

		void submit(CommandBuffer* cmdBuffer);
	};
}

#endif
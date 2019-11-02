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
#include "CommandBuffer.h"
#include "GpuResource.h"
#include <set>
#include <RenderAPI\GpuResource.h>

namespace ge
{
	enum CommandBufferTypeFlags 
	{
		CBT_MAIN = 1 << 0,
		CBT_TRANSFER = 1 << 1,
		CBT_COMPUTE = 1 << 2,
		CBT_ALL = 0xFF
	};

	class CommandBuffer;
	class GpuContext
	{
		friend class					 CommandBuffer;
		std::set<CommandBuffer*>	     m_commandBuffers;
		std::vector<RPtr<CommandBuffer>> m_submitedBuffers;
		RPtr<CommandBuffer>				 m_transferCb;
		RPtr<CommandBuffer>				 m_mainCb;
	protected:
		virtual void		submit(CommandBuffer* cmdBuffer) = 0;
	public:
		static void			setCurrentGpuContext(Ptr<GpuContext> context);
		static GpuContext&	instance();

		void				releaseResource(const RPtr<GpuResource>& resource);
		CommandBuffer&		mainCb();
		CommandBuffer&		transferCb();
		void				sumbit(CommandBufferTypeFlags types);

		virtual ~GpuContext() = default;

		virtual Uniform*	createUniform(const UNIFORM_DESC& desc) = 0;
		virtual Texture2D*	createTexture2D(const TEXTURE2D_DESC& desc) = 0;
		virtual Buffer*		createBuffer(const BUFFER_DESC& desc) = 0;
		virtual Sampler*	createSampler(const SAMPLER_DESC& desc) = 0;

		virtual CommandBuffer* createCommandBuffer(const COMMAND_BUFFER_DESC& desc) = 0;
		virtual Pipeline*	createPipeline(const PIPELINE_DESC& desc) = 0;
		virtual Framebuffer* createFramebuffer(const FRAMEBUFFER_DESC& desc) = 0;
		virtual usize		allignUniform(usize length) const = 0;

	    
	};
}

#endif
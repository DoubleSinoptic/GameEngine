#pragma once
#ifndef __GPUCONTEXT_H__
#define __GPUCONTEXT_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

#include "Sampler.h"
#include "Texture.h"
#include "Buffer.h"
#include "Uniform.h"
#include "Pipeline.h"
#include "Framebuffer.h"
#include "GpuContext.h"

namespace ge
{
	struct TEXTURE2D_COPY_DESC
	{
		uint32 mipLevel;
		uint32 arrayLayer;
		uint32 width;
		uint32 heght;
		uint32 x;
		uint32 y;
	};

	template<typename T>
	struct ClearColor4
	{
		T x, y, z, w;
	};

	union CLEAR_COLOR
	{
		ClearColor4<scalar>  scalars;
		ClearColor4<int32>   sint;
		ClearColor4<uint32>  uint;
	};

	class GpuContext
	{
	public:
		static void setCurrentGpuContext(Ptr<GpuContext> context);
		static GpuContext& instance();

		virtual ~GpuContext() = default;

		virtual Uniform* createUniform(const UNIFORM_DESC& desc) = 0;
		virtual Texture2D* createTexture2D(const TEXTURE2D_DESC& desc) = 0;
		virtual Buffer* createBuffer(const BUFFER_DESC& desc) = 0;
		virtual Sampler* createSampler(const SAMPLER_DESC& desc) = 0;

		virtual void copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart) = 0;;
		virtual void copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg) = 0;
		virtual void copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter) = 0;

		virtual void drawIndexed(uint32 firstVerteces, uint32 vertexes) = 0;
		virtual void draw(uint32 firstVerteces, uint32 vertexes) = 0;
		virtual void drawIndexedInstanced(uint32 firstVerteces, uint32 vertexes, uint32 firstInstance, uint32 instanced) = 0;

		virtual void submit() = 0;

		virtual void setClearColors(const CLEAR_COLOR* clearColors, usize num) = 0;

		virtual void setPipeline(Pipeline* pipeline) = 0;

		virtual void setFrameBuffer(Framebuffer* framebuffer) = 0;

		virtual void setUniforms(Uniform* const * uniform, uint32 start, uint32 num) = 0;

		void setUniform(Uniform* uniform, uint32 start = 0) 
		{
			setUniforms(&uniform, start, 1);
		}
	};
}

#endif
#pragma once
#ifndef __COMMADNBUFFER_H__
#define __COMMADNBUFFER_H__

#include "Core/Core.h"
#include "GpuResource.h"
#include "Sampler.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "Uniform.h"
#include "Pipeline.h"
#include "Framebuffer.h"

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



	struct CLEAR_DEPTH_STENCIL_VALUE {
		scalar       depth;
		uint32       stencil;
	};

	union CLEAR_COLOR_VALUE {
		scalar    float32[4];
		int32     int32[4];
		uint32    uint32[4];
	};

	union CLEAR_COLOR
	{		
		CLEAR_COLOR_VALUE         color;
		CLEAR_DEPTH_STENCIL_VALUE depthStencil;
	};

	enum QueueType 
	{
		QT_TRANSFER,
		QT_GRAPHICS,
		QT_COMPUTE,
		/**
		* ext*
		*/
		QT_PRESENT
	};

	struct COMMAND_BUFFER_DESC 
	{
		bool isSecondary = false;
		uint32 poolIndex = 0;
		QueueType queueType;
		/**
		* @brief
		* очень важный параметр для Low-level 
		* фреймворков, которые не умеют отслеживать,
		* удаленные на момент полёта, ресурсы.
		* 
		* Если стоит true то все ресурсы удаленные
		* на момент существования этого коммандного буфера
		* будут зарегестрированны в него. это очень важно
		* так-как не приходится производить трекинг
		* во время bindDescriptorSets (drawcall)
		*/
		bool	usePredictedResourceTrack = true;

		struct Hash 
		{
			size_t operator()(const COMMAND_BUFFER_DESC& desc) const noexcept 
			{
				return byteArrayHash((const ge::byte*)&desc, sizeof(COMMAND_BUFFER_DESC));
			}
		};
		struct Equal
		{
			bool operator()(const COMMAND_BUFFER_DESC& a, const COMMAND_BUFFER_DESC& b) const noexcept
			{
				if (a.isSecondary != b.isSecondary || 
					a.poolIndex != b.poolIndex || 
					a.queueType != b.queueType || 
					a.usePredictedResourceTrack != b.usePredictedResourceTrack)
					return false;
				return true;
			}
		};
	};
	class Swapchain;
	class CommandBuffer : public GpuResource
	{		
		COMMAND_BUFFER_DESC						m_desc;
		Vector<RPtr<const GpuResource>>			m_resources;
		bool									m_enabledTraking;
	public:
		~CommandBuffer();
		CommandBuffer(const COMMAND_BUFFER_DESC& desc, GpuContext* context);
		const COMMAND_BUFFER_DESC& getDesc() const;
		void trackResource(const RPtr<const GpuResource>& resource);
		
		virtual bool isFinished() = 0;
		virtual void copyBuffer(Buffer* dst, Buffer* src, usize size, usize dstStart, usize srcStart) = 0;;
		virtual void copyBufferToImage(Texture2D* dst, Buffer* src, usize size, usize srcStart, const TEXTURE2D_COPY_DESC* dstReg) = 0;
		virtual void copyImage(Texture2D* dst, Texture2D* src, const TEXTURE2D_COPY_DESC* dstReg, const TEXTURE2D_COPY_DESC* srcReg, SampledFilter filter) = 0;

		virtual void drawIndexed(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces) = 0;
		virtual void draw(uint32 firstVerteces, uint32 numVerteces) = 0;
		virtual void drawIndexedInstanced(uint32 firstIndeces, uint32 numIndeces, uint32 firstVerteces, uint32 firstInstnace, uint32 instancesNum) = 0;

		virtual void setIndecesBuffer(Buffer* buffer, uint32 offset, bool x32bit) = 0;
		virtual void setVertexBuffer(Buffer* vertexBuffer, uint32 first, uint32 offset) = 0;

		virtual void setClearColors(const CLEAR_COLOR* clearColors, usize num) = 0;
		virtual void setPipeline(Pipeline* pipeline) = 0;
		virtual void setFrameBuffer(Framebuffer* framebuffer) = 0;
		virtual void setUniforms(Uniform* const* uniform, uint32 start, uint32 num) = 0;
		virtual void setUniform(Uniform* uniform, uint32 start = 0) = 0;
		virtual void execute(CommandBuffer* secondryCommandBuffer) = 0;
		virtual void present(Swapchain* swapchain, Texture2D* image) = 0;
	};
}

#endif
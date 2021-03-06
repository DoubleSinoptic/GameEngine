#pragma once
#ifndef __UNIFORM_H__
#define __UNIFORM_H__

#include "Core/Core.h"
#include "GpuResource.h"

namespace ge
{
	class Texture2D;
	class Buffer;
	class Sampler;

	enum PipelineStage
	{
		PS_NONE = 0,
		PS_VERTEX = 1 << 0,
		PS_PIXEL = 1 << 1,
		PS_COMPUTE = 1 << 2
	};

	enum UniformType 
	{
		UT_NONE,
		UT_BUFFER,
		UT_SAMPLER,
		UT_TEXTURE		
	};

	struct DESCRIPTOR_DESC 
	{
		UniformType   type = UT_NONE;
		PipelineStage stages = PS_NONE;
	};

	struct UNIFORM_DESC 
	{
		DESCRIPTOR_DESC resources[16];
	};

	class Uniform : public GpuResource
	{
		UNIFORM_DESC m_desc;
	public:
		Uniform(const UNIFORM_DESC& desc, GpuContext* context) :
			m_desc(desc),
			GpuResource(context)
		{}

		constexpr const UNIFORM_DESC& getDesc() const noexcept
		{
			return m_desc;
		}

		virtual void setSampler(Sampler* sampler, int32 slot) = 0;
		virtual void setTexture(Texture2D* texture, int32 slot) = 0;
		virtual void setBuffer(Buffer* buffer, int32 slot) = 0;
		virtual void setBuffer(Buffer* buffer, usize offset, usize length, int32 slot) = 0;
	};
}

#endif
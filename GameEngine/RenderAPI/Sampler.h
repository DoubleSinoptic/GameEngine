#pragma once
#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

namespace ge
{
	enum SampledFilter 
	{
		SF_NEAREST,
		SF_LINEAR
	};

	enum AddressMod
	{
		AM_REPEAT,
		AM_MIRRORED_REPEAT,
		AM_MIRRORED_CLAMP_TO_BORDER,
		AM_MIRRORED_CLAMP_TO_EDGE,
		AM_CLAMP_TO_BORDER,
		AM_CLAMP_TO_EDGE
	};

	struct SAMPLER_DESC
	{
		SampledFilter	min = SF_NEAREST;
		SampledFilter   mag = SF_NEAREST;
		SampledFilter	mip = SF_NEAREST;
		scalar			anisotropy = 0.0;
		uint32			maxAnisotropy = 1.0;
		scalar			minLod = -1000.0;
		scalar			maxLod = 10000.0;
		AddressMod		addressModU = AM_REPEAT;
		AddressMod		addressModV = AM_REPEAT;
		AddressMod		addressModW = AM_REPEAT;
	};

	class Sampler : public ResourceObject
	{
		const SAMPLER_DESC m_desc;
	public:
		Sampler(const SAMPLER_DESC& desc) :
			m_desc(desc)
		{}

		constexpr const SAMPLER_DESC& getDesc() const noexcept
		{
			return m_desc;
		}
	};
}

#endif
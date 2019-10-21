#pragma once
#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

namespace ge
{
	enum TextureUsage 
	{
		TU_SAMPLED_IMAGE = 1 << 0,
		TU_COLORATTACHMENT = 1 << 1,
		TU_DEPTHATTACHMENT = 1 << 2
	};

	enum PixelFormat 
	{
		PF_R16G16B16A16_SNORM,
		PF_R16G16B16A16_SNORM,

		PF_R16G16B16_SNORM,
		PF_R16G16B16_SNORM,

		PF_R16G16_SNORM,
		PF_R16G16_SNORM,

		PF_R16_SNORM,
		PF_R16_SNORM,

		PF_R16G16B16A16_UNORM,
		PF_R16G16B16A16_UNORM,

		PF_R16G16B16_UNORM,
		PF_R16G16B16_UNORM,

		PF_R16G16_UNORM,
		PF_R16G16_UNORM,

		PF_R16_UNORM,
		PF_R16_UNORM,

		PF_R8G8B8A8_SNORM,
		PF_R8G8B8A8_SNORM,

		PF_R8G8B8_SNORM,
		PF_R8G8B8_SNORM,

		PF_R8G8_SNORM,
		PF_R8G8_SNORM,

		PF_R8_SNORM,
		PF_R8_SNORM,

		PF_R8G8B8A8_UNORM,
		PF_R8G8B8A8_UNORM,

		PF_R8G8B8_UNORM,
		PF_R8G8B8_UNORM,

		PF_R8G8_UNORM,
		PF_R8G8_UNORM,

		PF_R8_UNORM,
		PF_R8_UNORM,

		PF_R32G32B32A32_FLOAT,
		PF_R16G16B16A16_FLOAT,

		PF_R32G32B32_FLOAT,
		PF_R16G16B16_FLOAT,

		PF_R32G32B32_FLOAT,
		PF_R16G16B16_FLOAT,

		PF_R32G32_FLOAT,
		PF_R16G16_FLOAT,

		PF_R32_FLOAT,
		PF_R16_FLOAT,

		PF_D32,
		PF_D24,
		PF_D16,
		PF_D8
	};

	struct TEXTURE2D_DESC 
	{
		PixelFormat		format = PF_R8G8B8A8_UNORM;
		uint32			width = 1;
		uint32			height = 1;
		TextureUsage	usage = TU_SAMPLED_IMAGE;
		uint32			mipCount = 1;
		uint32			layerCount = 1;
	};

	class Texture2D : public ResourceObject
	{
		const TEXTURE2D_DESC m_desc;
	public:
		Texture2D(const TEXTURE2D_DESC& desc) :
			m_desc(desc)
		{}

		constexpr const TEXTURE2D_DESC& getDesc() const noexcept 
		{
			return m_desc;
		}
	};
}

#endif
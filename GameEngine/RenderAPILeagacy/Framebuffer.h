#pragma once
#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Core/Core.h"
#include "GpuResource.h"
#include "Uniform.h"

namespace ge
{
	class Texture2D;

	struct RENDERTARGET_DESC 
	{
		RPtr<Texture2D>	texture;
		uint32		mip = 0;
		uint32		layer = 0;
	};

	struct FRAMEBUFFER_DESC 
	{
		uint32				width = 1;
		uint32				heigth = 1;
		usize				colorAttachmentsNum;
		RENDERTARGET_DESC	colorAttachments[16];
		RENDERTARGET_DESC	depth;
		RENDERTARGET_DESC	stencil;
	};

	class Framebuffer : public GpuResource
	{
		const FRAMEBUFFER_DESC		 m_desc;
	public:
		Framebuffer(const FRAMEBUFFER_DESC& desc, GpuContext* context) :
			m_desc(desc),
			GpuResource(context)
		{}

		constexpr const FRAMEBUFFER_DESC& getDesc() const noexcept
		{
			return m_desc;
		}
	};
}

#endif
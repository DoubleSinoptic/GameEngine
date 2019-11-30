#pragma once
#ifndef __SWAPCHAIN_H__
#define __SWAPCHAIN_H__

#include "Core/Core.h"
#include "GpuResource.h"

namespace ge
{
	class Texture2D;

	class Swapchain : public GpuResource
	{
	public:
		Swapchain(GpuContext* context) :
			GpuResource(context)
		{}

		//constexpr const UNIFORM_DESC& getDesc() const noexcept
		//{
		//	return m_desc;
		//}

		virtual Texture2D* getTexture() = 0;
	};
}

#endif
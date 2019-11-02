#pragma once
#ifndef __GPURESOURCE_H__
#define __GPURESOURCE_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

namespace ge
{
	class GpuContext;
	class GpuResource : public ResourceObject
	{
		mutable bool m_isReleased;
		GpuContext*  m_context;
	public:
		GpuResource(GpuContext* context);

		void markAsReleased() const noexcept;

		constexpr GpuContext& gpuContext() const noexcept 
		{
			return *m_context;
		}

		template<typename T>
		constexpr T& gpuContextT() const noexcept
		{
			return static_cast<T&>(*m_context);
		}

		virtual void release() const noexcept override;
	};
}

#endif
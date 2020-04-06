#pragma once
#ifndef __GPURESOURCE_H__
#define __GPURESOURCE_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

namespace ge
{
	class CommandBuffer;
	class GpuContext;
	class GpuResource : public ResourceObject
	{
		mutable bool   m_isReleased;
		GpuContext*    m_context;
		CommandBuffer* m_writeOwner;
	public:
		GpuResource(GpuContext* context);

		void markAsReleased() const noexcept;

		constexpr GpuContext& gpuContext() const noexcept 
		{
			return *m_context;
		}

		CommandBuffer* getWriteOwner() const;
		void setWriteOwner(CommandBuffer* cmdBuffer);

		template<typename T>
		constexpr T& gpuContextT() const noexcept
		{
			return static_cast<T&>(*m_context);
		}

		virtual void release() const noexcept override;
	};
}

#endif
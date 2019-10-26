#pragma once
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"

namespace ge
{
	enum MemoryType 
	{
		MT_STATIC,
		MT_DYNAMIC
	};

	enum BufferUsage 
	{
		BU_UNDEFINED = 0 << 0,
		BU_VERTECES = 1 << 0,
		BU_INDECES = 1 << 1,
		BU_STORAGE = 1 << 2,
		BU_UNIFORM = 1 << 3,
	};

	enum AccessFlags 
	{
		AF_READ = 1 << 0,
		AF_WRITE = 1 << 1
	};

	struct BUFFER_DESC 
	{
		usize size = 256;
		MemoryType memType = MT_DYNAMIC;
		BufferUsage usage = BU_UNIFORM;
	};

	class Buffer : public ResourceObject
	{
		BUFFER_DESC m_desc;
	public:
		virtual ~Buffer() = default;

		Buffer(const BUFFER_DESC& desc) noexcept :
			m_desc(desc)
		{}

		constexpr const BUFFER_DESC& getDesk() const noexcept
		{
			return m_desc;
		}
		virtual void* map(usize offset, usize length, AccessFlags flags) = 0;
		virtual void unmap() = 0;
	};
}

#endif
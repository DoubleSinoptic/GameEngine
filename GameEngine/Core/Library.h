#pragma once
#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "./Core.h"
#include "./Any.h"

namespace ge
{
	class Library
	{
		Any m_module;
	public:
		Library(const String& title) noexcept;

		bool isLoad() const noexcept;

		void* getProcessAddressPtr(const String& path) const noexcept;

		template<typename Fn>
		Fn getProcessAddress(const String& path) const noexcept
		{
			return reinterpret_cast<Fn>(getProcessAddressPtr(path));
		}
	};
}

#endif


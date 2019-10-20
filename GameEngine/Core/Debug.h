#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Core.h"
#include "StringFormat.h"

namespace ge
{
	class Debug 
	{
	public:
		template<typename FormatCharType, typename... Args>
		static void log(const FormatCharType* str, const Args&... args) 
		{
			struct T0 : public ExceptPutProvider
			{
				virtual void put(char16_t c) override
				{
					putchar(c);
				}
			} ge;
			putFormat<FormatCharType, Args...>(&ge, str, args...);
			ge.put('\n');
		}

		template<typename FormatCharType, typename... Args>
		static void log1(const FormatCharType* str, const Args& ... args)
		{
			struct T0 : public ExceptPutProvider
			{
				virtual void put(char16_t c) override
				{
					putchar(c);
				}
			} ge;
			putFormat<FormatCharType, Args...>(&ge, str, args...);
			ge.put('\n');
		}
	};
}

#endif
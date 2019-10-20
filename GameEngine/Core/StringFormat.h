#pragma once
#ifndef __STRINGFORMAT_H__
#define __STRINGFORMAT_H__

#include "./Core.h"

namespace ge
{
	class ExceptPutProvider
	{
	public:
		virtual ~ExceptPutProvider() = default;
		virtual void put(char16_t c) = 0;
	};

	struct PutRefBase
	{
		virtual ~PutRefBase() = default;
		virtual void put(ExceptPutProvider* prov) const = 0;
	};

	GE_EXPORT void genericPut(ExceptPutProvider* provider, float value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, double value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, long value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, long long value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, unsigned long value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, unsigned long long value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, int value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, unsigned int value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, short value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, unsigned short value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, char value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, unsigned char value);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, const char* p);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, const wchar_t* p);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, const char16_t* p);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, const String& s);
	GE_EXPORT void genericPut(ExceptPutProvider* provider, const void* p);

	template<typename T>
	void genericPut(typename std::enable_if<ToStringChecker<T>::has, ExceptPutProvider*>::type provider, const T& value)
	{
		String result = value.toString();
		for (auto character : result)
			provider->put(character);
	}

	template<typename T>
	struct PutRefT : public PutRefBase
	{
		const T& v;
		PutRefT(const T& x) noexcept : v(x)
		{}
		virtual ~PutRefT() = default;
		virtual void put(ExceptPutProvider* prov) const
		{
			genericPut(prov, v);
		}
	};

	struct PutRefContainer
	{
		byte longData[sizeof(PutRefT<String>)];
		template<typename T>
		PutRefContainer(const T& value)
		{
			new(longData) PutRefT<T>(value);
		}

		void put(ExceptPutProvider* prov) const
		{
			reinterpret_cast<const PutRefBase*>(longData)->put(prov);
		}
	};

	template<typename FormatCharType, typename... Args>
	void putFormat(ExceptPutProvider* put, const FormatCharType* c, const Args& ... v)
	{
		PutRefContainer puts[] = { v... };
		char numberBuffer[5];
		for (; *c != FormatCharType('\0'); c++) {
			if (*c == FormatCharType('{')) {
				c++;
				FormatCharType bufferIndex = 0;
				for (; *c != FormatCharType('}'); c++) {
					numberBuffer[bufferIndex++] = char(*c);
				}
				numberBuffer[bufferIndex] = '\0';
				int index = atoi(numberBuffer);
				puts[index].put(put);
			}
			else {
				put->put(*c);
			}
		}
	}

	template<typename FormatCharType>
	void putFormat(ExceptPutProvider* put, const FormatCharType* c)
	{
		while (*c) {
			put->put(*c);
			c++;
		}
	}

	template<typename BufferCharType, typename FormatCharType, typename... Args>
	void formatBuffer(BufferCharType* buffer, const FormatCharType* c, const Args& ... v)
	{
		struct T0 : public ExceptPutProvider
		{
			BufferCharType* s;
			virtual void put(char16_t c) override
			{
				*s++ = c;
			}
		} ge;
		ge.s = buffer;
		putFormat<FormatCharType, Args...>(&ge, c, v...);
		*ge.s++ = BufferCharType('\0');
	}

	template<typename FormatCharType, typename... Args>
	String format(const FormatCharType* c, const Args& ... v)
	{
		struct T0 : public ExceptPutProvider
		{
			usize i = 0;
			virtual void put(char16_t c) override
			{
				i++;
			}
		} counterPut;
		putFormat<FormatCharType, Args...>(&counterPut, c, v...);
		String s(counterPut.i, char16_t('x'));
		struct T1 : public ExceptPutProvider
		{
			String* s;
			usize i = 0;
			virtual void put(char16_t c) override
			{
				s->begin()[i++] = c;
			}
		} stringPut;
		stringPut.s = &s;
		putFormat<FormatCharType, Args...>(&stringPut, c, v...);
		return s;
	}
}

#endif
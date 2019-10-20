#include "StringFormat.h"

namespace ge 
{

	template<typename T>
	void staticPut(ExceptPutProvider* provider, T value, const char* qual)
	{
		char temp[60];
		temp[0] = '\0';
		int result = snprintf(temp, 60, qual, value);
		geAssert(result >= 0);
		const char* p = temp;
		for (; *p != '\0'; p++)
			provider->put(char16_t(*p));
	}

	void genericPut(ExceptPutProvider* provider, float value)
	{
		staticPut(provider, value, "%f");
	}

	void genericPut(ExceptPutProvider* provider, double value)
	{
		staticPut(provider, value, "%lf");
	}

	void genericPut(ExceptPutProvider* provider, long value)
	{
		staticPut(provider, value, "%ld");
	}

	void genericPut(ExceptPutProvider* provider, long long value)
	{
		staticPut(provider, value, "%lld");
	}

	void genericPut(ExceptPutProvider* provider, unsigned long value)
	{
		staticPut(provider, value, "%lu");
	}

	void genericPut(ExceptPutProvider* provider, unsigned long long value)
	{
		staticPut(provider, value, "%llu");
	}

	void genericPut(ExceptPutProvider* provider, int value)
	{
		staticPut(provider, value, "%d");
	}

	void genericPut(ExceptPutProvider* provider, unsigned int value)
	{
		staticPut(provider, value, "%u");
	}

	void genericPut(ExceptPutProvider* provider, short value)
	{
		staticPut(provider, value, "%hd");
	}

	void genericPut(ExceptPutProvider* provider, unsigned short value)
	{
		staticPut(provider, value, "%hu");
	}

	void genericPut(ExceptPutProvider* provider, char value)
	{
		staticPut(provider, value, "%hhd");
	}

	void genericPut(ExceptPutProvider* provider, unsigned char value)
	{
		staticPut(provider, value, "%hhu");
	}

	void genericPut(ExceptPutProvider* provider, const char* p)
	{
		const byte* iter = reinterpret_cast<const byte*>(p);
		uint32 c = 0;
		while (c = ImplUtf8::utf8CharIterator(iter)) {
			provider->put(char16_t(c));
		}
	}

	void genericPut(ExceptPutProvider* provider, const wchar_t* p)
	{
		for (; *p != '\0'; p++)
			provider->put(char16_t(*p));
	}

	void genericPut(ExceptPutProvider* provider, const char16_t* p)
	{
		for (; *p != '\0'; p++)
			provider->put(char16_t(*p));
	}

	void genericPut(ExceptPutProvider* provider, const String& s)
	{
		const char16_t* p = s.data();
		for (; *p != '\0'; p++)
			provider->put(char16_t(*p));
	}

	void genericPut(ExceptPutProvider* provider, const void* p)
	{
		staticPut(provider, p, "%px");
	}
}
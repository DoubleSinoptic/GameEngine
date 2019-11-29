#include "Core.h"
#include "MessageBox.h"
#include "StringFormat.h"

namespace ge
{
	usize ImplUtf8::charLengthInUtf8(uint32 c) noexcept
	{
		if (c >> 7 == 0)
			return 1;
		if (c >> 11 == 0)
			return 2;
		if (c >> 16 == 0)
			return 3;
		if (c >> 21 == 0)
			return 4;
		return -1;
	}

	usize ImplUtf8::writeUtf8Char(byte* buffer, uint32 c) noexcept
	{
		usize sz = charLengthInUtf8(c);
		switch (sz)
		{
		case 1:
			buffer[0] = c;
			break;
		case 2:
			buffer[0] = 0b11000000 | byte(c >> 6) & 0b00011111;
			buffer[1] = 0b10000000 | byte(c) & 0b00111111;
			break;
		case 3:
			buffer[0] = 0b11100000 | byte(c >> 12) & 0b00001111;
			buffer[1] = 0b10000000 | byte(c >> 6) & 0b00111111;
			buffer[2] = 0b10000000 | byte(c) & 0b00111111;
			break;
		case 4:
			buffer[0] = 0b11110000 | byte(c >> 16) & 0b00000111;
			buffer[1] = 0b10000000 | byte(c >> 12) & 0b00111111;
			buffer[2] = 0b10000000 | byte(c >> 6) & 0b00111111;
			buffer[3] = 0b10000000 | byte(c) & 0b00111111;
			break;
		}
		return sz;
	}

	usize ImplUtf8::utf8CharLength(const byte sz) noexcept
	{
		if (sz >> 7 == 0)
			return 1;
		if (sz >> 5 == 0b110)
			return 2;
		if (sz >> 4 == 0b1110)
			return 3;
		if (sz >> 3 == 0b11110)
			return 4;
		return 0;
	}

	uint32 ImplUtf8::utf8CharIterator(const byte*& iter) noexcept
	{
		char part0 = 0, part1 = 0, part2 = 0, part3 = 0;
		uint32 c = 0;
		usize lngth = utf8CharLength(*iter);
		switch (lngth)
		{
		case 1:
			c = *iter & 0b01111111;
			iter++;
			break;
		case 2:
			part0 = *iter & 0b00011111;
			iter++;
			part1 = *iter & 0b00111111;
			iter++;
			c = (uint32(part0) << 6) | uint32(part1);
			break;
		case 3:
			part0 = *iter & 0b00001111;
			iter++;
			part1 = *iter & 0b00111111;
			iter++;
			part2 = *iter & 0b00111111;
			iter++;
			c = (uint32(part0) << 12) | (uint32(part1) << 6) | uint32(part2);
			break;
		case 4:
			part0 = *iter & 0b00000111;
			iter++;
			part1 = *iter & 0b00111111;
			iter++;
			part2 = *iter & 0b00111111;
			iter++;
			part3 = *iter & 0b00111111;
			iter++;
			c = (uint32(part0) << 16) | (uint32(part1) << 12) + (uint32(part2) << 6) | uint32(part3);
			break;
		case 0:
			geAssertFalse("Invalid utf8 code.");
		}
		return c;
	}

	String utf8(const char* utf8str)
	{
		String resultString;
		const byte* iter = reinterpret_cast<const byte*>(utf8str);
		uint32 character = 0;
		while (character = ImplUtf8::utf8CharIterator(iter))
		{
			resultString.push_back(char16_t(character));
		}
		return resultString;
	}

	Utf8String to_utf8(const char16_t* string) 
	{
		Utf8String resultString;
		while (*string)
		{
			byte buffer[4];
			const usize encodeLength = ImplUtf8::writeUtf8Char(buffer, uint32(*string++));
			for (usize i = 0; i < encodeLength; i++)
				resultString.push_back(buffer[i]);
		}
		resultString.push_back('\0');
		return resultString;
	}

	String utf8(const Utf8String& utf8str)
	{
		return utf8(utf8str.c_str());
	}

	Utf8String to_utf8(const String& string)
	{
		return to_utf8(string.c_str());
	}

	usize byteArrayHash(const byte* ptr, usize len)
	{	
		usize seed = 0x0256;
		const usize m = 0x5bd1e995;
		usize hash = seed ^ len;
		const byte* buf = static_cast<const byte*>(ptr);

		while (len >= 4)
		{
			usize k = *reinterpret_cast<const usize*>(buf);
			k *= m;
			k ^= k >> 24;
			k *= m;
			hash *= m;
			hash ^= k;
			buf += 4;
			len -= 4;
		}

		switch (len)
		{
		case 3:
			hash ^= static_cast<byte>(buf[2]) << usize(16);
		case 2:
			hash ^= static_cast<byte>(buf[1]) << usize(8);
		case 1:
			hash ^= static_cast<byte>(buf[0]);
			hash *= m;
		};

		hash ^= hash >> 13;
		hash *= m;
		hash ^= hash >> 15;
		return hash;	
	}

	bool byteArrayEqual(const byte* a, const byte* b, usize dataSize)
	{
		
		return false;
	}

	void nativeAssert(bool expr, const char* str, const char* file, int line)
	{
		if (expr)
			return;
		String errorMessage = format("         GameEngine submited critical error.        \n\nfile: {0}\n\tline: {1}\n{2}", utf8(file), line, str);
		MessageBox::showInfo(errorMessage, u"GameEngine submited critical error", MBT_ERROR);
		exit(-1);
	}
}
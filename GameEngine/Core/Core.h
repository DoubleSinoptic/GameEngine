#pragma once
#ifndef __CORE_H__
#define __CORE_H__

#include <string>
#include <cstdint>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <memory>
#include <functional>
#include <stdexcept>
#include <atomic>
#include <algorithm>
#include <cassert>

#define GE_EXPORT
#define NONDTR_STATIC

#ifdef _EXCPETIONS
#	ifdef _DEBUG
#		define geAssertDebug(x) {if (!(x)) { throw std::runtime_error(# x); }}
#	else
#		define geAssertDebug(x)
#	endif

#	define geAssert(x) {if (!(x)) { throw std::runtime_error(# x); }}
#	define geAssertFalse(x)  {if (!(x)) { throw std::runtime_error(x); }}
#else
	namespace ge
	{
		void nativeAssert(bool expr, const char* str, const char*file, int line);
	}
#	ifdef _DEBUG
#		define geAssertDebug(x) nativeAssert(x, # x, __FILE__, __LINE__)
#	else
#		define geAssertDebug(x)
#	endif

#	define geAssert(x) nativeAssert(x, # x, __FILE__, __LINE__)
#	define geAssertFalse(x) nativeAssert(false, x, __FILE__, __LINE__)
#endif
namespace ge
{
	template <typename T>
	class ToStringChecker
	{
		template <typename C> static std::true_type  test(decltype(&C::toString));
		template <typename C> static std::false_type test(...);
	public:
		static constexpr bool has = decltype(test<T>(0))::value;
	};

	enum FundumentalType 
	{
		FT_FLOAT,
		FT_DOUBLE,
		FT_UINT32,
		FT_INT32,
		FT_UINT16,
		FT_INT16,
		FT_UINT8,
		FT_INT8,
		FT_UINT64,
		FT_INT64
	};

	typedef std::basic_string<char16_t> String;
	typedef std::basic_string<char> Utf8String;

	String utf8(const char* utf8str);
	Utf8String to_utf8(const char16_t* string);

	String utf8(const Utf8String& utf8str);
	Utf8String to_utf8(const String& string);

	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	using Queue = std::queue<T>;

	template<typename T>
	using List = std::list<T>;

	template<typename T>
	using Ptr = std::shared_ptr<T>;

	typedef int32_t int32;
	typedef int16_t int16;
	typedef int8_t  int8;
	typedef int64_t int64;

	typedef uint32_t uint32;
	typedef uint16_t uint16;
	typedef uint8_t  uint8;
	typedef uint64_t uint64;

	typedef uint8_t  byte;
	typedef int8_t  sbyte;

	typedef size_t usize;
	typedef ptrdiff_t ssize;
	typedef float scalar;

	constexpr usize InvalidPlace = usize(~1);

	constexpr uint8 UInt8Max = 0xFF;
	constexpr int8 Int8Max = 0x7F;

	constexpr uint16 UInt16Max = 0xFFFF;
	constexpr int16 Int16Max = 0x7FFF;

	constexpr uint32 UInt32Max = 0xFFFFFFFF;
	constexpr int32 Int32Max = 0x7FFFFFFF;

	constexpr uint64 UInt64Max = 0xFFFFFFFFFFFFFFFF;
	constexpr int64 Int64Max = 0x7FFFFFFFFFFFFFFF;

	template<typename T, typename... Args>
	Ptr<T> snew(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	class ImplUtf8
	{
	public:
		static usize charLengthInUtf8(uint32 c) noexcept;
		static usize writeUtf8Char(byte* buffer, uint32 c) noexcept;
		static usize utf8CharLength(const byte sz) noexcept;
		static uint32 utf8CharIterator(const byte*& iter) noexcept;
	};

	template<typename T>
	bool removeSwap(Vector<T>& vec, usize index)
	{
		vec[index] = vec.back();
		vec.pop_back();
		return vec.size() != index;
	}

	template<typename T>
	void removeFind(Vector<T>& vec, const T& key)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), key), vec.end());
	}

	template<typename T>
	inline const char* getTypeName()
	{
		return typeid(T).name();
	}

	template<typename T>
	int32 compareBinary(const T& a, const T& b)
	{
		usize count = sizeof(T);
		const byte* s1 = reinterpret_cast<const byte*>(&a);
		const byte* s2 = reinterpret_cast<const byte*>(&a);
		while (count-- > 0)
		{
			if (*s1++ != *s2++)
				return s1[-1] < s2[-1] ? -1 : 1;
		}
		return 0;
	}
}

#endif
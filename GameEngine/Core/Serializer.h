#pragma once
#ifndef	__BINRAYSERIALIZER_H__
#define __BINRAYSERIALIZER_H__

#include "Core.h"

namespace ge
{
	class BinaryWriter;
	class BinaryReader;

	class ISerializable
	{
	public:
		virtual ~ISerializable() {}
		virtual void serialize(BinaryWriter* writer) const = 0;
		virtual void deserialize(BinaryReader* reader) = 0;
	};

	class BinaryWriter
	{
	public:
		virtual ~BinaryWriter() = default;
		virtual bool writeBytes(const byte* data, usize length) = 0;
		virtual uint32 instanceId(ISerializable* typeId) = 0;

		template<typename T>
		constexpr void write(const T& v)
		{
			writeBytes(reinterpret_cast<const byte*>(&v), sizeof(T));
		}

		template<typename T>
		void writeN(const T* v, usize count)
		{
			writeBytes(reinterpret_cast<const byte*>(v), sizeof(T) * count);
		}

		void writeString(const String& str)
		{
			write<uint64>(str.size());
			writeN(str.data(), str.size());
		}

		template<typename T>
		void writeVector(const Vector<T>& v)
		{
			write<uint64>(v.size());
			writeN(v.data(), v.size());
		}
	};

	class BinaryReader
	{
	public:
		virtual ~BinaryReader() = default;
		virtual bool readBytes(byte* data, usize length) = 0;
		virtual ISerializable* instanceNew(uint32 typeId) = 0;
		virtual String getFwd() const = 0;

		template<typename T>
		constexpr T read()
		{
			T v;
			readBytes(reinterpret_cast<byte*>(&v), sizeof(T));
			return v;
		}

		template<typename T>
		void readN(T* v, usize count)
		{
			readBytes(reinterpret_cast<byte*>(v), sizeof(T) * count);
		}

		String readString()
		{
			uint64 size = read<uint64>();
			String str(size, '=');
			readN((char16_t*)str.data(), size);
			return str;
		}

		template<typename T>
		Vector<T> readVector()
		{
			uint64 size = read<uint64>();
			Vector<T> str(size);
			readN(str.data(), size);
			return str;
		}
	};

	class MemoryWriter : public BinaryWriter 
	{
		std::vector<byte> m_result;
	public:
		const std::vector<byte>& get() const;
		virtual bool writeBytes(const byte* data, usize length) override;
		virtual uint32 instanceId(ISerializable* typeId) override;
	};

	class MemoryReader : public BinaryReader
	{
		const byte* m_mem;
		const usize m_length;
		usize		m_offset;
	public:
		MemoryReader(const Vector<byte>& values);
		MemoryReader(const byte* memoty, usize length);
		virtual bool readBytes(byte* data, usize length) override;
		virtual ISerializable* instanceNew(uint32 typeId) override;
		virtual String getFwd() const override;
		void reset();
	};

}

#endif
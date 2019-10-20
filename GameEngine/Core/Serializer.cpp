#include "Serializer.h"
namespace ge 
{
	const std::vector<byte>& MemoryWriter::get() const
	{
		return m_result;
	}

	bool MemoryWriter::writeBytes(const byte* data, usize length)
	{
		m_result.insert(m_result.end(), data, data + length);
		return true;
	}

	uint32 MemoryWriter::instanceId(ISerializable* typeId)
	{
		return uint32();
	}

	MemoryReader::MemoryReader(const Vector<byte>& values) :
		m_mem(values.data()),
		m_length(values.size()),
		m_offset(0)
	{
	}

	MemoryReader::MemoryReader(const byte* memoty, usize length) :
		m_mem(memoty),
		m_length(length),
		m_offset(0)
	{}

	bool MemoryReader::readBytes(byte* data, usize length)
	{
		if (length + m_offset > m_length)
			return false;
		memcpy(data, m_mem + m_offset, length);
		m_offset += length;
		return true;
	}

	ISerializable* MemoryReader::instanceNew(uint32 typeId)
	{
		return nullptr;
	}

	String MemoryReader::getFwd() const
	{
		return String();
	}

	void MemoryReader::reset()
	{
		m_offset = 0;
	}

}

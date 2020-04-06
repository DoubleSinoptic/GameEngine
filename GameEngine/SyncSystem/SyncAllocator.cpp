#include "SyncAllocator.h"

namespace ge
{

	SyncAllocator::SyncAllocator() :
		m_frames{ std::vector<byte>(1024 * 1024, ' ') },
		m_offset(0)
	{
		resetBack();
	}

	void SyncAllocator::resetBack()
	{
		m_max = m_frames.back().size();
		m_cur = m_frames.back().data();
		m_offset = 0;
	}

	void SyncAllocator::reset()
	{
		if (m_frames.size() > 1)
		{
			size_t maxSize = 0;
			for (auto& c : m_frames)
			{
				maxSize += c.size();
			}
			m_frames.clear();
			m_frames.push_back(std::vector<byte>(maxSize));
		}
		resetBack();
	}

	void* SyncAllocator::allocate(usize r)
	{
		const usize n = (r + 15) & ~15;
		if (m_offset + n >= m_max)
		{
			m_frames.push_back(Vector<byte>(m_max * 2));
			resetBack();
			return allocate(n);
		}
		byte* p = m_cur + m_offset;
		m_offset += n;
		return p;
	}
}
#pragma once
#ifndef __INDEXEDSTORAGE_H__
#define __INDEXEDSTORAGE_H__

#include "./Core.h"

namespace ge
{
	template<typename T>
	class IndexedStorage 
	{
		std::vector<T>		m_storage;
		std::vector<usize>	m_freeIndeces;
	public:
		const T& query(usize index) const 
		{
			geAssertDebug(index < m_storage.size());
			return m_storage[index];
		}

		const T& operator[](usize index) const 
		{
			geAssertDebug(index < m_storage.size());
			return m_storage[index];
		}

		template<typename X>
		usize insert(X&& v) 
		{
			if (m_freeIndeces.empty())
			{
				usize index = m_storage.size();
				m_storage.emplace_back(std::forward<X>(v));
				return index;
			}
			usize index = m_freeIndeces.back();
			m_freeIndeces.pop_back();
			m_storage[index] = std::forward<X>(v);
			return index;
		}

		void free(usize index) 
		{
			m_storage[index] = T();
			m_freeIndeces.push_back(index);
		}
	};
}

#endif
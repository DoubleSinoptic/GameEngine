#pragma once
#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

#include "Core.h"

#include <list>
#include <mutex>
#include <memory>

namespace ge
{
	template<typename T>
	class ObjectPool
	{
		std::mutex m_lock;

		struct PooledObject
		{
			Ptr<T> object;
			usize  age;
		};

		List<PooledObject> m_pool;
	public:
		typedef Ptr<T> pointer_type;

		void collect(usize age) 
		{
			std::lock_guard<std::mutex> _(m_lock);
			Vector<typename List<PooledObject>::iterator> freeObjects;
			for (auto i = m_pool.begin(); i != m_pool.end(); i++)
			{
				i->age++;
				if (i->age > age)
					freeObjects.push_back(i);
			}
			for (auto& x : freeObjects)
				m_pool.erase(x);
		}

		void free(const pointer_type& x)
		{
			std::lock_guard<std::mutex> _(m_lock);
			m_pool.push_back({ x, 0 });
		}

		template<typename Predicate>
		pointer_type allocate_great(const Predicate& pred)
		{
			std::lock_guard<std::mutex> _(m_lock);
			for (auto i = m_pool.begin(); i != m_pool.end(); i++)
			{
				if (pred(i->object))
				{
					auto val = std::move(i->object);
					m_pool.erase(i);
					return val;
				}
			}
			return pointer_type(new T());
		}

		pointer_type allocate()
		{
			std::lock_guard<std::mutex> _(m_lock);
			if (m_pool.size())
			{
				pointer_type val = std::move(m_pool.back().object);
				m_pool.pop_back();
				return val;
			}
			return pointer_type(new T());
		}
	};

}
#endif
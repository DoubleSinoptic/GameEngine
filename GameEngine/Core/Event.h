#pragma once
#ifndef __EVENT_H__
#define __EVENT_H__

#include "./Core.h"
#include <map>
#include <functional>

namespace ge
{
	typedef uint32 CallbackId;
	class Event
	{
		CallbackId										   m_nextId;
		std::map<CallbackId, std::function<void(void)>> m_functions;
	public:	
		Event() : m_nextId(0)
		{}

		typedef std::function<void(void)> function_type;

		void operator ()() const 
		{
			for (auto& x : m_functions)
				x.second();
		}

		template<typename T>
		CallbackId insert(T&& f) 
		{
			CallbackId id = m_nextId++;
			m_functions.emplace(id, std::forward<T>(f));
			return id;
		}

		void remove(CallbackId id) 
		{
			m_functions.erase(id);
		}
	};
}

#endif
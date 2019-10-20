#pragma once
#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

#include "Core.h"
#include "ObjectPool.h"

#include <functional>

namespace ge 
{
	class CommandQueue
	{
		std::mutex										    m_lock;
		ObjectPool<Vector<std::function<void(void)>>>	    m_queues;
		Ptr<Vector<std::function<void(void)>>>				m_currentQueue;
	public:
		CommandQueue();

		void playback();

		template<typename X, typename A, typename... Args>
		void queue(X o, const A& f, Args&& ... args);

		template<typename F>
		void queue(F&& f);
	};

	template<typename X, typename A, typename... Args>
	void CommandQueue::queue(X o, const A& f, Args&& ... args)
	{
		std::lock_guard<std::mutex> _(m_lock);
		m_currentQueue->push_back([=]() {
			(o->*f)(std::forward<Args>(args)...);
			});
	}

	template<typename F>
	void CommandQueue::queue(F&& f)
	{
		std::lock_guard<std::mutex> _(m_lock);
		m_currentQueue->push_back(f);
	}
}

#endif
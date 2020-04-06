#pragma once
#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

#include "Core.h"
#include "ObjectPool.h"

#include <functional>

namespace ge 
{
	class SpinLock 
	{
		std::atomic_bool m_locked;
	public:
		SpinLock(bool createLocked = false);

		bool try_lock();

		void unlock();

		void lock();
	};

	struct CommandQueueTaskBase {
		CommandQueueTaskBase* next;
		CommandQueueTaskBase() : next(nullptr)
		{}
		virtual ~CommandQueueTaskBase() = default;
		virtual void exec() const = 0;
	};

	template<typename X>
	struct CommandQueueTaskT : public CommandQueueTaskBase {
		X object;
		template<typename T>
		CommandQueueTaskT(T&& value) noexcept :
			object(std::forward<T>(value))
		{}
		virtual void exec() const override
		{
			object();
		}
	};

	class CommandQueue
	{
		std::atomic_size_t	  m_opCount;
		std::mutex			  m_lock;
		CommandQueueTaskBase* m_left;
		CommandQueueTaskBase* m_right;

	    void addTask(CommandQueueTaskBase* t) noexcept;
	public:
		CommandQueue();

		void playback();

		~CommandQueue();

		template<typename X, typename A, typename... Args>
		void queue(X o, const A& f, const Args&... args);

		template<typename F>
		void queue(F&& f);
	};

	template<typename X, typename A, typename... Args>
	void CommandQueue::queue(X o, const A& f, const Args&... args)
	{
		std::lock_guard<std::mutex> _(m_lock);
		auto callable = [=]() {
			(o->*f)(args...);
		};
		CommandQueueTaskBase* task = new CommandQueueTaskT<decltype(callable)>(std::move(callable));
		addTask(task);
	}

	template<typename F>
	void CommandQueue::queue(F&& f)
	{
		std::lock_guard<std::mutex> _(m_lock);
		CommandQueueTaskBase* task = new CommandQueueTaskT<typename std::decay<F>::type>(std::forward<F>(f));
		addTask(task);
	}
}

#endif
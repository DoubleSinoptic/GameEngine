#pragma once
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Core.h"
#include "Thread.h"
#include <mutex>
#include <condition_variable>

namespace ge
{
	enum BarrierType
	{
		BT_STRONG,
		BT_RELXAED
	};
	class ThreadPool 
	{
		std::mutex								m_lock;
		std::condition_variable					m_var;
		std::atomic_bool						m_isClosed;
		Vector<Ptr<Thread>>						m_threads;
		std::queue<std::function<void(void)>>	m_tasks;
		void threadMain();
	public:
		~ThreadPool();
		ThreadPool();

		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;

		static void setCurrentThreadPool(Ptr<ThreadPool> pool);
		static ThreadPool& instance();

		usize threadCount() const noexcept;
		void runTask(std::function<void(void)>&&);
		void runTask(const std::function<void(void)>&);
		void parallelFor(usize beg, usize end, const std::function<void(usize beg, usize end)>& fn);

		void barrier(BarrierType waitType);
	};
}

#endif
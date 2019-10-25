#include "ThreadPool.h"

namespace ge 
{
	Ptr<ThreadPool> currentThreadPool;

	ThreadPool::~ThreadPool()
	{
		std::unique_lock<std::mutex> _(m_lock);
		m_isClosed.store(true);
		m_var.notify_all();
	}

	ThreadPool::ThreadPool() :
		m_isClosed(false)
	{
		for (usize i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			m_threads.push_back(snew<Thread>([=]() { while (!m_isClosed.load()) threadMain(); }));
		}
	}

	void ThreadPool::setCurrentThreadPool(Ptr<ThreadPool> pool)
	{
		currentThreadPool = pool;
	}

	ThreadPool& ThreadPool::instance()
	{
		return *currentThreadPool;
	}

	usize ThreadPool::threadCount() const noexcept
	{
		return m_threads.size();
	}

	void ThreadPool::threadMain()
	{
		std::function<void(void)> f;
		{
			std::unique_lock<std::mutex> _(m_lock);
			while (!m_isClosed.load()) {
				if (!m_tasks.empty()) {
					f = std::move(m_tasks.front());
					m_tasks.pop();
					break;
				}
				m_var.wait(_);
			}
		}
		if (f)
			f();
	}

	void ThreadPool::runTask(std::function<void(void)>&& f)
	{
		std::unique_lock<std::mutex> _(m_lock);
		m_tasks.emplace(std::move(f));
		m_var.notify_one();
	}

	void ThreadPool::runTask(const std::function<void(void)>& f)
	{
		std::unique_lock<std::mutex> _(m_lock);
		m_tasks.push(f);
		m_var.notify_one();
	}

	void ThreadPool::parallelFor(usize beg, usize end, const std::function<void(usize beg, usize end)>& fn)
	{
		/*const usize distance = end - beg;
		const usize workers = threadCount() - 1;
		const usize div = distance / workers;
		const usize mod = distance % workers;
		std::atomic_int resultCount = 0;
		usize next = beg;
		if (mod > 0) {
			for (usize i = 0; i < workers; i++, next += div)
				runTask([&resultCount, next, &fn, div]() {
				fn(next, next + div);
				resultCount += 1;
					});
			fn(next, next + mod);
			while (resultCount.load() != workers)
				std::this_thread::yield();
		}*/
	
	
	}

	void ThreadPool::barrier(BarrierType waitType)
	{
		if (waitType == BT_RELXAED)
		{
			while (true)
			{
				std::unique_lock<std::mutex> _(m_lock);
				if (!m_tasks.size())
					return;
				Thread::sleep(1);
			}
		}
		else
		{
			while (m_tasks.size())
			{
				Thread::yield();
			}
		}

	}
}

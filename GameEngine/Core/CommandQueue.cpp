#include "CommandQueue.h"

namespace ge 
{
	CommandQueue::CommandQueue() :
		m_opCount(0)
	{
		m_currentQueue = m_queues.allocate([]() {
			return snew<Vector<std::function<void(void)>>>();
		});
	}

	void CommandQueue::playback()
	{
		while (m_opCount.load())
		{
			std::shared_ptr<Vector<std::function<void(void)>>> queue;
			{
				std::lock_guard<std::mutex> _(m_lock);
				queue = m_currentQueue;
				m_currentQueue = m_queues.allocate([]() {
					return snew<Vector<std::function<void(void)>>>();
					});
			}
			size_t count = 0;
			for (auto& f : *queue)
			{
				count++;				
				if (f)
					f();
			}
			queue->clear();
			m_queues.free(queue);		
	
			m_opCount.fetch_sub(count);
			
		}
	}

	SpinLock::SpinLock(bool createLocked) :
		m_locked(createLocked)
	{}

	bool SpinLock::try_lock()
	{
		bool flase_bool = false;
		return m_locked.compare_exchange_weak(flase_bool, true);
	}

	void SpinLock::unlock()
	{
		m_locked.store(false);
	}

	void SpinLock::lock()
	{
		while (!try_lock())
			std::this_thread::yield();
	}

}
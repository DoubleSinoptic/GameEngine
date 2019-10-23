#include "CommandQueue.h"

namespace ge 
{
	CommandQueue::CommandQueue()
	{
		m_currentQueue = m_queues.allocate([]() {
			return snew<Vector<std::function<void(void)>>>();
		});
	}

	void CommandQueue::playback()
	{
		std::shared_ptr<Vector<std::function<void(void)>>> queue;
		{
			std::lock_guard<std::mutex> _(m_lock);
			queue = m_currentQueue;
			m_currentQueue = m_queues.allocate([]() {
				return snew<Vector<std::function<void(void)>>>();
			});
		}
		for (auto& f : *queue)
		{
			if (f)
				f();
		}
		queue->clear();
		m_queues.free(queue);
	}

}
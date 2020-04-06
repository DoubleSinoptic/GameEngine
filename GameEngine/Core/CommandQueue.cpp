#include "CommandQueue.h"

namespace ge 
{


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

	 void CommandQueue::addTask(CommandQueueTaskBase* t) noexcept {
		if (m_left)
		{
			m_right->next = t;
			m_right = t;
		}
		else
		{
			m_left = t;
			m_right = t;
		}
	}

	CommandQueue::CommandQueue() :
		m_left(nullptr),
		m_right(nullptr)
	{}

	void CommandQueue::playback()
	{
		CommandQueueTaskBase* left = nullptr;
		{
			std::lock_guard<std::mutex> _(m_lock);
			left = m_left;
			m_left = nullptr;
			m_right = nullptr;
		}
		while (left) {
			CommandQueueTaskBase* nextTask = left->next;
			left->exec();
			delete left;
			left = nextTask;
		}
	}

	CommandQueue::~CommandQueue()
	{
		std::lock_guard<std::mutex> _(m_lock);
		while (m_left) {
			CommandQueueTaskBase* nextTask = m_left->next;
			delete m_left;
			m_left = nextTask;
		}
		m_right = nullptr;
	}

}
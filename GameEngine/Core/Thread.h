#pragma once
#ifndef __THREAD_H__
#define __THREAD_H__

#include "Core.h"
#include <thread>

namespace ge
{
	class NONDTR_STATIC GE_EXPORT Thread
	{
		std::atomic_uint32_t		m_id;
		Ptr<std::thread>			m_thread;
	public:
		Thread(const std::function<void(void)>& f);

		Thread(Thread&&) = delete;
		Thread& operator=(Thread&&) = delete;

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		static void sleep(uint32 ms);
		static void yield();

		void close();

		static uint32 currentId() noexcept;
		static bool isClosed() noexcept;
		uint32 id() const noexcept;

		~Thread();
	};

	class Fence
	{
		std::atomic_uint32_t m_flag;
	public:
		Fence() noexcept : m_flag(0)
		{}

		void reset() noexcept
		{
			m_flag.store(0);
		}

		void signal() noexcept
		{
			m_flag.fetch_add(1);
		}

		void wait() const noexcept
		{
			while (!m_flag.load())
			{
				Thread::yield();
			}
		}

		void wait(uint32 count) const noexcept
		{
			while (m_flag.load() < count)
			{
				Thread::yield();
			}
		}
	};

}

#endif
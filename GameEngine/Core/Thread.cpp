#include "Thread.h"
#include "MessageBox.h"
#include "Debug.h"
#include <map>
#include <atomic>
#ifdef _WIN32
#	include <Windows.h>
#	undef MessageBox
#endif
namespace ge
{
	std::atomic_uint32_t       m_incrementer(0);
	std::map<uint32, bool>     m_isDestroyed;
	thread_local uint32		   m_thisId;

	Thread::Thread(const std::function<void(void)>& f) :
		m_id(m_incrementer++)
	{
		m_isDestroyed[m_id] = false;
		uint32 selectedId = m_id;
		m_thread = snew<std::thread>([=]() {
			m_thisId = selectedId;
			try {
				Debug::log("Thread started: {0}", selectedId);
				f();
			}
			catch (const std::exception& ex) {
				MessageBox::showInfo(utf8(ex.what()), u"GameEngine error.", MBT_ERROR);
			}
			Debug::log("Thread destroyed: {0}", selectedId);
		});
	}

	void Thread::setThreadMarker(const String& name)
	{
#ifdef _WIN32
		SetThreadDescription(GetCurrentThread(), (const wchar_t*)name.c_str());
#else
		(void)name;
#endif
	}

	void Thread::sleep(uint32 ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	void Thread::yield()
	{
		std::this_thread::yield();
	}

	void Thread::close()
	{
		m_isDestroyed[m_id] = true;
	}

	void Thread::join()
	{
		if (m_thread->joinable())
			m_thread->join();
	}

	uint32 Thread::currentId() noexcept
	{
		return m_thisId;
	}

	bool Thread::isClosed() noexcept
	{
		return m_isDestroyed[currentId()];
	}

	uint32 Thread::id() const noexcept
	{
		return m_id;
	}

	Thread::~Thread()
	{
		close();
		if (m_thread->joinable())
			m_thread->join();
		m_isDestroyed.erase(m_id);
	}
}


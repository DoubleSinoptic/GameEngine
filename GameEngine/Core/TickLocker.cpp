#include "TickLocker.h"
#include "Time.h"
#include "Thread.h"

namespace ge 
{
	TickLocker::TickLocker(uint64 delta) :
		m_delta(delta),
		m_lastLockTime(0)
	{}

	void TickLocker::setDelta(uint64 dt) noexcept
	{
		m_delta = dt;
	}

	uint64 TickLocker::getDelta() const noexcept
	{
		return m_delta;
	}

	void TickLocker::lock()
	{
		if (m_delta) 
		{
			const uint64 nextLockTime = m_lastLockTime + m_delta;
			uint64 currentTime = Time::totalTicks();
			if (currentTime < nextLockTime)
			{
				uint64 lost = nextLockTime - m_lastLockTime;
				if (lost > SleepRefusionTime)
					Thread::sleep((lost - SleepRefusionTime) / 1000);
				do 
				{
					currentTime = Time::totalTicks();
				} while (currentTime < nextLockTime);
			}
			m_lastLockTime = currentTime;
		}
	}

}


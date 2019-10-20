#pragma once
#ifndef __TICKLOCKER_H__
#define __TICKLOCKER_H__

#include "./Core.h"

namespace ge
{
	constexpr uint64 SleepRefusionTime = 2000;

	class GE_EXPORT TickLocker 
	{
		uint64 m_lastLockTime;
		uint64 m_delta;
	public:
		TickLocker(uint64 delta = 1000000/60);
		void setDelta(uint64 dt) noexcept;
		uint64 getDelta() const noexcept;
		void lock();
	};
}

#endif
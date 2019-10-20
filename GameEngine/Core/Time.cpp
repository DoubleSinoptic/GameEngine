#include "Time.h"
#include <chrono>

namespace ge 
{
	typedef std::chrono::high_resolution_clock clock_type;
	clock_type::time_point appStart = clock_type::now();

	uint64 Time::totalTicks()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(clock_type::now() - appStart).count();
	}

	uint64 Time::deltaTick(uint64& cache)
	{
		const uint64 current = totalTicks();
		const uint64 delta = current - cache;
		cache = current;
		return delta;
	}
}

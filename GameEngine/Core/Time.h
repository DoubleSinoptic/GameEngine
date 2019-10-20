#pragma once
#ifndef __TIME_H__
#define __TIME_H__

#include "./Core.h"

namespace ge
{
	class Time 
	{
	public:
		static uint64 totalTicks();
		static uint64 deltaTick(uint64& cache);
	};
}

#endif
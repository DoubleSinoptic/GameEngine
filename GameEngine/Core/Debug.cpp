#include "Debug.h"
#include <mutex>
namespace ge 
{
	std::mutex debugLock;
	void Debug::lock()
	{
		debugLock.lock();
	}

	void Debug::unlock()
	{
		debugLock.unlock();
	}
}

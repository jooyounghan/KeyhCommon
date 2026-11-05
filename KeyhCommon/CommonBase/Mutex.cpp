#include "CommonBasePch.h"
#include "Mutex.h"

namespace keyh
{
	void Mutex::lock()
	{
		_mutex.lock();
	}

	bool Mutex::tryLock()
	{
		return _mutex.try_lock();
	}

	void Mutex::unlock()
	{
		_mutex.unlock();
	}
}

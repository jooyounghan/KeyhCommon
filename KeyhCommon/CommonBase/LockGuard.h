#pragma once
#include "Mutex.h"

namespace keyh
{
	class LockGuard
	{
	public:
		explicit LockGuard(Mutex& mutex)
			: _mutex(mutex)
		{
			_mutex.lock();
		}

		~LockGuard()
		{
			_mutex.unlock();
		}

		REMOVE_COPY_AND_MOVE(LockGuard);

	private:
		Mutex& _mutex;
	};
}

#pragma once
#include "CommonCore.h"
#include <mutex>

namespace keyh
{
	class ConditionalVariable;

	class Mutex
	{
		friend class ConditionalVariable;

	public:
		Mutex() = default;
		~Mutex() = default;
		REMOVE_COPY_AND_MOVE(Mutex);

		void lock();
		bool tryLock();
		void unlock();

	private:
		std::mutex _mutex;
	};
}

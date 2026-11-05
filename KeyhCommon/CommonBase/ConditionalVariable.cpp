#include "CommonBasePch.h"
#include "ConditionalVariable.h"

namespace keyh
{
	void ConditionalVariable::wait(Mutex& mutex)
	{
		std::unique_lock<std::mutex> lock(mutex._mutex, std::adopt_lock);
		_variable.wait(lock);
		lock.release();
	}

	void ConditionalVariable::notifyOne()
	{
		_variable.notify_one();
	}

	void ConditionalVariable::notifyAll()
	{
		_variable.notify_all();
	}
}

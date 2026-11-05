#pragma once
#include "Mutex.h"
#include <condition_variable>

namespace keyh
{
	class ConditionalVariable
	{
	public:
		ConditionalVariable() = default;
		~ConditionalVariable() = default;
		REMOVE_COPY_AND_MOVE(ConditionalVariable);

		// The calling thread must hold mutex before waiting.
		void wait(Mutex& mutex);
		template<typename Predicate> void wait(Mutex& mutex, Predicate&& predicate);
		void notifyOne();
		void notifyAll();

	private:
		std::condition_variable _variable;
	};
}
#include "ConditionalVariable.hpp"

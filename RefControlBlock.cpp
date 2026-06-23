#include "CommonBasePch.h"
#include "RefControlBlock.h"

namespace keyh
{
	inline bool releaseRefCount(std::atomic<int>& refCount)
	{
		int count = refCount.load(std::memory_order_relaxed);
		while (true)
		{
			std::memory_order success_order = (count == 1) ? std::memory_order_acq_rel : std::memory_order_release;
			if (refCount.compare_exchange_weak(count, count - 1, success_order, std::memory_order_relaxed))
			{
				return count == 1;
			}
		}
	}

	bool RefControlBlock::release()
	{
		return releaseRefCount(_refCount);
	}

	bool RefControlBlock::releaseWeak()
	{
		return releaseRefCount(_weakRefCount);
	}
}

#pragma once
#include "CommonCore.h"

namespace keyh
{
	class RefControlBlock
	{
	private:
		std::atomic<int> _refCount = 0;
		std::atomic<int> _weakRefCount = 0;

	public:
		inline void addRef() { _refCount.fetch_add(1, std::memory_order_relaxed); }
		inline void addWeakRef() { _weakRefCount.fetch_add(1, std::memory_order_relaxed); }
		inline int getRefCount() const { return _refCount.load(std::memory_order_relaxed); }

	public:
		bool release();
		bool releaseWeak();
	};
}
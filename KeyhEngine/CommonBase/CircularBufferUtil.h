#pragma once
#include "CommonCore.h"
#include "MemoryUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	struct CircularBufferUtil
	{
	public:
		inline static size_t getIndex(size_t index, ptrdiff_t offset, size_t capacity) noexcept
		{
			KEYH_ASSERT(MemoryUtil::hasSingleBit(capacity), "Capacity must be a power of 2 for getIndex to work correctly.");
			return (index + offset) % capacity;
		}
	};
}


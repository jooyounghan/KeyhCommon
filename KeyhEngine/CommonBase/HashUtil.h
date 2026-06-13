#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct HashUtil
	{
		constexpr static size_t kInitialCapacity = 8;
		constexpr static float kMaxLoadFactor = 0.75f;
		constexpr static int32 kEmptyPsl = -1;

	public:
		static uint32 foldHash(size_t hash) noexcept
		{
			return static_cast<uint32>(hash ^ (hash >> 32));
		}
	};
}
#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct HashUtil
	{
		static constexpr size_t kInitialCapacity = 8;
		static constexpr float kMaxLoadFactor = 0.75f;
		static constexpr int32 kEmptyPsl = -1;
		static constexpr size_t kInvalidHash = static_cast<size_t>(-1);

		enum class InsertStatus
		{
			Inserted,
			Replaced,
			AlreadyExists,
			Error
		};

		static uint32 foldHash(size_t hash) noexcept
		{
			return static_cast<uint32>(hash ^ (hash >> 32));
		}
	};
}
#pragma once
#include "TypeCommon.h"
#include "ClassCommon.h"
#include "HashUtil.h"
#include "MathUtil.h"
#include "StaticArray.h"

namespace keyh
{
	struct HashContainer
	{
		constexpr static size_t kInitialCapacity = 7;
		constexpr static float kMaxLoadFactor = 0.75f;
		constexpr static int32 kEmptyPsl = -1;
		constexpr static size_t kHashContainerCapacityTableSize = 25;

	public:
		FORCE_INLINE static size_t getFastRangeIndex(size_t hash, size_t capacity)
		{
#if defined(_MSC_VER) && defined(_WIN64)
			unsigned __int64 highResult = 0;
			_umul128(hash, capacity, &highResult);
			return (size_t)highResult;
#elif defined(__SIZEOF_INT128__)
			unsigned __int128 product = (unsigned __int128)hash * capacity;
			return (size_t)(product >> 64);
#else
			return hash % capacity;
#endif
		}

	public:
		static constexpr const StaticArray<int32, kHashContainerCapacityTableSize>& getHashContainerCapacityTable()
		{
			return _capacityTable;
		}

	private:
		static constexpr StaticArray<int32, kHashContainerCapacityTableSize> createHashContainerCapacityTable() noexcept
		{
			StaticArray<int32, kHashContainerCapacityTableSize> table{};
			for (size_t i = 0; i < kHashContainerCapacityTableSize; ++i)
			{
				table[i] = static_cast<int32>(MathUtil::nextPrime(kInitialCapacity * (1ULL << i)));
			}
			return table;
		}

		static const StaticArray<int32, kHashContainerCapacityTableSize> _capacityTable;
	};

	inline constexpr StaticArray<int32, HashContainer::kHashContainerCapacityTableSize> HashContainer::_capacityTable = HashContainer::createHashContainerCapacityTable();
}
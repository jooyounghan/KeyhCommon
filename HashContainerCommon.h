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
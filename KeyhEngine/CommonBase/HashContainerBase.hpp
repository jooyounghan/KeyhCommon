#include "HashContainerBase.h"
#pragma once
#define GET_CAPACITY_TABLE(table) constexpr const StaticArray<int32, HashUtil::kHashUtilCapacityTableSize>& table = HashUtil::getHashUtilCapacityTable();
#define GET_CAPACITY_TABLE_SIZE HashUtil::kHashUtilCapacityTableSize

namespace keyh
{
	template<typename Derived>
	template<typename Key, typename ...Args>
	auto HashContainerBase<Derived>::insertImpl(bool replace, Key&& key, Args && ...args)
	{
		rehashIfNeeded();

		Derived* self = static_cast<Derived*>(this);
		size_t hash = self->_hasher(key);
		size_t index = HashUtil::getFastRangeIndex(hash, _capacity);

		auto& bucket = self->_buckets[index];
		if (bucket.isEmpty())
		{
			self->constructBucket(bucket, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
			++_size;
		}

		return self->makeInsertResult(bucket, InsertStatus::Inserted);
	}

	template<typename Derived>
	void HashContainerBase<Derived>::rehashIfNeeded()
	{
		if (HashUtil::kMaxLoadFactor * _capacity <= _size)
		{
			GET_CAPACITY_TABLE(capacityTable);
			_capacity = _capacity == 0 ? HashUtil::kInitialCapacity : capacityTable[_capacityLevel++];
			static_cast<Derived*>(this)->rehash(_capacity);
		}
	}

	template<typename Derived>
	void HashContainerBase<Derived>::reserve(size_t newCapacity)
	{
		GET_CAPACITY_TABLE(capacityTable);
		for (size_t i = 0; i < GET_CAPACITY_TABLE_SIZE; ++i)
		{
			if (newCapacity <= capacityTable[i])
			{
				_capacityLevel = static_cast<uint8>(i);
				break;
			}
		}
		_capacity = newCapacity;
		static_cast<Derived*>(this)->rehash(_capacity);
	}
}

#undef GET_CAPACITY_TABLE
#undef GET_CAPACITY_TABLE_SIZE
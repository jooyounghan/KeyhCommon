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

		int32 psl = 0;

		auto& bucket = self->_buckets[index];
		if (bucket.isEmpty())
		{
			self->constructBucket(bucket, psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
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
			size_t newCapacity = _capacity == 0 ? HashUtil::kInitialCapacity : capacityTable[_capacityLevel++];
			rehash(newCapacity);
		}
	}

	template<typename Derived>
	void HashContainerBase<Derived>::rehash(size_t newCapacity)
	{
		using Bucket = typename Derived::Bucket;

		Derived* self = static_cast<Derived*>(this);
		Bucket* oldBuckets = self->_buckets;
		size_t oldCapacity = _capacity;
		self->_buckets = new Bucket[newCapacity]();
		_capacity = newCapacity;
		_size = 0;
		for (size_t i = 0; i < oldCapacity; ++i)
		{
			auto& bucket = oldBuckets[i];
			if (bucket.isEmpty())
				continue;

			self->insertImpl(false, keyh::move(bucket.key()), keyh::move(bucket.value()));
			bucket.~Bucket();
		}
		delete[] oldBuckets;
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
		rehash(newCapacity);
	}

	template<typename Derived>
	void HashContainerBase<Derived>::clear()
	{
		Derived* self = static_cast<Derived*>(this);
		for (size_t i = 0; i < _capacity; ++i)
		{
			auto& bucket = self->_buckets[i];
			if (!bucket.isEmpty())
			{
				bucket.~Bucket();
			}
		}
		delete[] self->_buckets;
		self->_buckets = nullptr;
		_capacity = 0;
		_capacityLevel = 0;
		_size = 0;
	}
}

#undef GET_CAPACITY_TABLE
#undef GET_CAPACITY_TABLE_SIZE
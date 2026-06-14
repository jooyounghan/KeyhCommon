#include "HashContainerBase.h"
namespace keyh
{
	template<typename Derived>
	template<typename Key, typename ...Args>
	auto HashContainerBase<Derived>::insertImpl(bool replace, Key&& key, Args && ...args)
	{
		rehashIfNeeded();

		Derived* self = static_cast<Derived*>(this);
		size_t hash = self->_hasher(key);
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		int32 psl = 0;
		InsertStatus status = InsertStatus::Error;
		typename Derived::Bucket* bucket = &self->_buckets[index];
		if (bucket->isEmpty())
		{
			// If the bucket is empty, insert the new key-value pair directly.
			bucket->constructBucket(psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
			status = InsertStatus::Inserted;
		}
		else
		{
			// If the bucket's key matches the new key, update the value if replace is true, otherwise do nothing.
			if (bucket->key() == key)
			{
				if (replace)
				{
					bucket->changeBucketValue(keyh::forward<Args>(args)...);
					status = InsertStatus::Replaced;
				}
				else
				{
					status = InsertStatus::AlreadyExists;
				}
			}
			else
			{
				typename Derived::Bucket tempBucket;
				tempBucket.constructBucket(psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
				size_t currentIndex = index;
				do
				{
					// If the bucket's PSL is less than the new key's PSL, swap the bucket with the new key-value pair and continue to insert the displaced bucket.
					if (bucket->getPsl() < psl)
					{
						tempBucket.setPsl(psl);
						psl = bucket->getPsl();
						tempBucket.swapBucket(bucket);						
					}
					else
					{
						++psl;
					}
					currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
					bucket = &self->_buckets[currentIndex];
				} while (currentIndex != index);
			}
		}

		if (status == InsertStatus::Inserted)
			++_size;

		KEYH_ASSERT(status != InsertStatus::Error, "HashContainerBase::insertImpl: Insertion failed");
		return self->makeInsertResult(bucket, status);
	}

	template<typename Derived>
	template<typename BucketT>
	auto HashContainerBase<Derived>::insertBucket(BucketT&& otherBucket)
	{
		rehashIfNeeded();

		Derived* self = static_cast<Derived*>(this);
		size_t hash = self->_hasher(otherBucket.key());
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		int32 psl = 0;
		InsertStatus status = InsertStatus::Error;
		typename Derived::Bucket* bucket = &self->_buckets[index];
		if (bucket->isEmpty())
		{
			// If the bucket is empty, insert the new key-value pair directly.
			otherBucket.setPsl(psl);
			bucket->swapBucket(&otherBucket);
			status = InsertStatus::Inserted;
		}
		else
		{
			// If the bucket's key matches the new key, update the value if replace is true, otherwise do nothing.
			if (bucket->key() == otherBucket.key())
			{
				bucket->~Bucket();
				bucket->swapBucket(&otherBucket);
				status = InsertStatus::Replaced;
			}
			else
			{
				size_t currentIndex = index;
				do
				{
					// If the bucket's PSL is less than the new key's PSL, swap the bucket with the new key-value pair and continue to insert the displaced bucket.
					if (bucket->getPsl() < psl)
					{
						otherBucket.setPsl(psl);
						psl = bucket->getPsl();
						otherBucket.swapBucket(bucket);
					}
					else
					{
						++psl;
					}
					currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
					bucket = &self->_buckets[currentIndex];
				} while (currentIndex != index);
			}
		}

		if (status == InsertStatus::Inserted)
			++_size;

		KEYH_ASSERT(status != InsertStatus::Error, "HashContainerBase::insertImpl: Insertion failed");
		return self->makeInsertResult(bucket, status);
	}


	template<typename Derived>
	template<typename Key>
	auto HashContainerBase<Derived>::findImpl(const Key& key)
	{
		Derived* self = static_cast<Derived*>(this);
		size_t hash = self->_hasher(key);
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		typename Derived::Bucket* bucket = &self->_buckets[index];
		if (bucket->isEmpty())
			return self->makeFindResult(nullptr, false);

		size_t currentIndex = index;
		do
		{
			if (bucket->key() == key)
			{
				return self->makeFindResult(bucket, true);
			}
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		return self->makeFindResult(nullptr, false);
	}

	template<typename Derived>
	template<typename Key>
	bool HashContainerBase<Derived>::removeImpl(const Key& key)
	{
		Derived* self = static_cast<Derived*>(this);
		size_t hash = self->_hasher(key);
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		typename Derived::Bucket* bucket = &self->_buckets[index];
		if (bucket->isEmpty())
			return false;

		size_t currentIndex = index;
		do
		{
			if (bucket->key() == key)
			{
				bucket->~Bucket();
				--_size;
				size_t nextIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
				typename Derived::Bucket* nextBucket = &self->_buckets[nextIndex];
				while (nextBucket->isEmpty() == false && nextBucket->getPsl() != 0)
				{
					nextBucket->swapBucket(bucket);
					bucket = nextBucket;
					nextIndex = CircularBufferUtil::getIndex(nextIndex, 1, _capacity);
					nextBucket = &self->_buckets[nextIndex];
				}
				return true;
			}
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		return false;
	}

	template<typename Derived>
	void HashContainerBase<Derived>::rehashIfNeeded()
	{
		if (HashUtil::kMaxLoadFactor * _capacity <= _size)
		{
			size_t newCapacity = _capacity == 0 ? HashUtil::kInitialCapacity : _capacity * 2;
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

			insertBucket(keyh::move(bucket));
			bucket.~Bucket();
		}

		if (oldBuckets)
			delete[] oldBuckets;
	}

	template<typename Derived>
	void HashContainerBase<Derived>::reserve(size_t newCapacity)
	{
		newCapacity = MemoryUtil::fastBitCeil(newCapacity);
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
		_size = 0;
	}
}
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

		typename Derived::Bucket* bucket = &self->_buckets[index];
		size_t currentIndex = index;
		int32 searchPsl = 0;
		do
		{
			if (bucket->isEmpty() || bucket->getPsl() < searchPsl)
				break;

			if (bucket->key() == key)
			{
				if (replace)
				{
					bucket->changeBucketValue(keyh::forward<Args>(args)...);
					return self->makeInsertResult(bucket, InsertStatus::Replaced);
				}

				return self->makeInsertResult(bucket, InsertStatus::AlreadyExists);
			}

			++searchPsl;
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		int32 psl = 0;
		InsertStatus status = InsertStatus::Error;
		typename Derived::Bucket tempBucket;
		tempBucket.constructBucket(psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
		bucket = &self->_buckets[index];
		currentIndex = index;
		do
		{
			if (bucket->isEmpty())
			{
				tempBucket.setPsl(psl);
				bucket->swapBucket(&tempBucket);
				status = InsertStatus::Inserted;
				break;
			}
			if (bucket->getPsl() < psl)
			{
				const int32 displacedPsl = bucket->getPsl();
				tempBucket.setPsl(psl);
				tempBucket.swapBucket(bucket);
				psl = displacedPsl;
			}

			++psl;
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

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

		typename Derived::Bucket* bucket = &self->_buckets[index];
		size_t currentIndex = index;
		int32 searchPsl = 0;
		do
		{
			if (bucket->isEmpty() || bucket->getPsl() < searchPsl)
				break;

			if (bucket->key() == otherBucket.key())
			{
				otherBucket.setPsl(bucket->getPsl());
				bucket->swapBucket(&otherBucket);
				return self->makeInsertResult(bucket, InsertStatus::Replaced);
			}

			++searchPsl;
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		int32 psl = 0;
		InsertStatus status = InsertStatus::Error;
		bucket = &self->_buckets[index];
		currentIndex = index;
		do
		{
			if (bucket->isEmpty())
			{
				otherBucket.setPsl(psl);
				bucket->swapBucket(&otherBucket);
				status = InsertStatus::Inserted;
				break;
			}
			if (bucket->getPsl() < psl)
			{
				const int32 displacedPsl = bucket->getPsl();
				otherBucket.setPsl(psl);
				otherBucket.swapBucket(bucket);
				psl = displacedPsl;
			}

			++psl;
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		if (status == InsertStatus::Inserted)
			++_size;

		KEYH_ASSERT(status != InsertStatus::Error, "HashContainerBase::insertImpl: Insertion failed");
		return self->makeInsertResult(bucket, status);
	}


	template<typename Derived>
	template<typename Key>
	auto HashContainerBase<Derived>::findImpl(const Key& key)
	{
		using Bucket = typename Derived::Bucket;

		Derived* self = static_cast<Derived*>(this);
		if (_capacity == 0)
			return self->makeFindResult(nullptr, false);

		size_t hash = self->_hasher(key);
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		Bucket* bucket = &self->_buckets[index];
		size_t currentIndex = index;
		int32 searchPsl = 0;
		do
		{
			if (bucket->isEmpty() || bucket->getPsl() < searchPsl)
				break;

			if (bucket->key() == key)
			{
				return self->makeFindResult(bucket, true);
			}

			++searchPsl;
			currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			bucket = &self->_buckets[currentIndex];
		} while (currentIndex != index);

		return self->makeFindResult(nullptr, false);
	}

	template<typename Derived>
	template<typename Key>
	bool HashContainerBase<Derived>::removeImpl(const Key& key)
	{
		using Bucket = typename Derived::Bucket;

		Derived* self = static_cast<Derived*>(this);
		if (_capacity == 0)
			return false;

		size_t hash = self->_hasher(key);
		size_t index = CircularBufferUtil::getIndex(hash, 0, _capacity);

		Bucket* bucket = &self->_buckets[index];
		size_t currentIndex = index;
		int32 searchPsl = 0;
		do
		{
			if (bucket->isEmpty() || bucket->getPsl() < searchPsl)
				break;

			if (bucket->key() == key)
			{
				Bucket* holeBucket = bucket;
				--_size;

				size_t nextIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
				Bucket* nextBucket = &self->_buckets[nextIndex];
				while (!nextBucket->isEmpty() && nextBucket->getPsl() > 0)
				{
					nextBucket->swapBucket(holeBucket);
					holeBucket->setPsl(holeBucket->getPsl() - 1);
					holeBucket = nextBucket;
					nextIndex = CircularBufferUtil::getIndex(nextIndex, 1, _capacity);
					nextBucket = &self->_buckets[nextIndex];
				}

				holeBucket->~Bucket();
				return true;
			}

			++searchPsl;
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
		using Bucket = typename Derived::Bucket;

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
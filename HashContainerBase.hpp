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
		// If the bucket is empty, insert the new key-value pair directly.
		if (bucket->isEmpty())
		{
			bucket->constructBucket(psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
			status = InsertStatus::Inserted;
		}
		else
		{
			size_t currentIndex = index;
			do
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
					break;
				}
				else
				{
					// If the bucket's PSL is less than the new key's PSL, swap the bucket with the new key-value pair and continue to insert the displaced bucket.
					if (bucket->_psl < psl)
					{
						typename Derived::Bucket tempBucket;
						tempBucket.swapBucket(bucket);
						bucket->constructBucket(psl, keyh::forward<Key>(key), keyh::forward<Args>(args)...);
						bucket = &tempBucket;
						psl = bucket->_psl;
						status = InsertStatus::Inserted;
					}
					else
					{
						++psl;
					}
				}
				currentIndex = CircularBufferUtil::getIndex(currentIndex, 1, _capacity);
			} while( currentIndex != index);
		}

		if (status == InsertStatus::Inserted)
			++_size;

		KEYH_ASSERT(status != InsertStatus::Error, "HashContainerBase::insertImpl: Insertion failed");
		return self->makeInsertResult(bucket, status);
	}

	template<typename Derived>
	template<typename Key>
	void HashContainerBase<Derived>::removeImpl(Key&& key)
	{
	
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

		for (size_t i = 0; i < oldCapacity; ++i)
		{
			auto& bucket = oldBuckets[i];
			if (bucket.isEmpty())
				continue;

			self->insertImpl(false, keyh::move(bucket.key()), keyh::move(bucket.value()));
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
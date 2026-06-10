#define HASHMAP_TEMPLATE_TYPE template<typename Key, typename Value, typename Hasher>
#define HASHMAP_CLASS HashMap<Key, Value, Hasher>
#define GET_CAPACITY_TABLE(table) constexpr const StaticArray<int32, HashContainer::kHashContainerCapacityTableSize>& table = HashContainer::getHashContainerCapacityTable();
#define GET_CAPACITY_TABLE_SIZE() HashContainer::kHashContainerCapacityTableSize
namespace keyh
{
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::~HashMap()
	{
		clear();
	}
	
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::HashMap(HashMap && other) noexcept
		: _buckets(other._buckets), _capacity(other._capacity), _size(other._size)
	{
		other._buckets = nullptr;
		other._capacity = 0;
		other._size = 0;
	}
	
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS& HASHMAP_CLASS::operator=(HashMap && other) noexcept
	{
		if (this != &other)
		{
			clear();
			_buckets = other._buckets;
			_size = other._size;
			_capacity = other._capacity;
			other._buckets = nullptr;
			other._capacity = 0;
			other._size = 0;
		}
		return *this;
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::insert(const Key& key, const Value& value, bool replace)
	{
		if (HashContainer::kMaxLoadFactor * _capacity <= _size)
		{
			GET_CAPACITY_TABLE(capacityTable);
			_capacityLevel = _capacity == 0 ? 0 : _capacityLevel + 1;
			_capacity = capacityTable[_capacityLevel];
			rehash(_capacity);
		}

		size_t hash = _hasher(key);
		size_t index = HashContainer::getFastRangeIndex(hash, _capacity);

		Bucket& bucket = _buckets[index];
		if (bucket->isEmpty())
		{
			bucket->_keyStorage = key;
			bucket->_valueStorage = value;
		}


	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::insert(Key&& key, Value&& value, bool replace)
	{
		if (HashContainer::kMaxLoadFactor * _capacity <= _size)
		{
			GET_CAPACITY_TABLE(capacityTable);
			_capacityLevel = _capacity == 0 ? 0 : _capacityLevel + 1;
			_capacity = capacityTable[_capacityLevel];
			rehash(_capacity);
		}

		size_t hash = _hasher(key);
		size_t index = HashContainer::getFastRangeIndex(hash, _capacity);

		Bucket& bucket = _buckets[index];
		if (bucket.isEmpty())
		{
			new (&bucket._keyStorage) Key(keyh::move(key));
			new (&bucket._valueStorage) Value(keyh::move(value));
		}

		return InsertResult{ bucket.value(), InsertStatus::Inserted };
	}

	HASHMAP_TEMPLATE_TYPE
	Value* HASHMAP_CLASS::find(const Key& key)
	{
		return nullptr;
	}

	HASHMAP_TEMPLATE_TYPE
	const Value* HASHMAP_CLASS::find(const Key& key) const
	{
		return nullptr;
	}

	HASHMAP_TEMPLATE_TYPE
	bool HASHMAP_CLASS::remove(const Key& key)
	{
		return false;
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::clear()
	{
	
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::reserve(size_t newCapacity)
	{
		GET_CAPACITY_TABLE(capacityTable);	
		constexpr size_t capacityTableSize = GET_CAPACITY_TABLE_SIZE();
		for (size_t i = 0; i < capacityTableSize; ++i)
		{
			if (newCapacity <= capacityTable[i])
			{
				_capacityLevel = static_cast<uint8>(i);
				break;
			}
		}
		_capacity = newCapacity;
		rehash(newCapacity);
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::rehash(size_t newCapacity)
	{

	}

}

#undef HASHMAP_TEMPLATE_TYPE
#undef HASHMAP_CLASS
#undef GET_CAPACITY_TABLE
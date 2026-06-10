#define HASHMAP_TEMPLATE_TYPE template<typename Key, typename Value, typename Hasher>
#define HASHMAP_CLASS HashMap<Key, Value, Hasher>

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
		return insertImpl(replace, key, value);
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::insert(Key&& key, Value&& value, bool replace)
	{
		return insertImpl(replace, move(key), move(value));
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
	void HASHMAP_CLASS::rehash(size_t newCapacity)
	{

	}
}

#undef HASHMAP_TEMPLATE_TYPE
#undef HASHMAP_CLASS
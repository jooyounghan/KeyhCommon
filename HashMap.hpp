#define HASHMAP_TEMPLATE_TYPE template<typename Key, typename Value, typename Hasher>
#define HASHMAP_CLASS HashMap<Key, Value, Hasher>

#include <utility>

namespace keyh
{
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::~HashMap()
	{
		clear();
	}
	
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::HashMap(HashMap && other) noexcept
		: Base(other._capacity, other._size), _buckets(other._buckets)
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
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::insert(const Key& key, const Value& value, bool replace, size_t* hashCache)
	{
		return insertImpl(replace, hashCache, key, value);
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::insert(Key&& key, Value&& value, bool replace, size_t* hashCache)
	{
		return insertImpl(replace, hashCache, keyh::move(key), keyh::move(value));
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::FindResult HASHMAP_CLASS::find(const Key& key, size_t* hashCache)
	{
		return findImpl(key, hashCache);
	}

	HASHMAP_TEMPLATE_TYPE
	bool HASHMAP_CLASS::remove(const Key& key, size_t* hashCache)
	{
		return removeImpl(key, hashCache);
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::makeInsertResult(Bucket* bucket, HashUtil::InsertStatus status)
	{
		return InsertResult(bucket->value(), status);
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::FindResult HASHMAP_CLASS::makeFindResult(Bucket* bucket, bool found)
	{
		return FindResult(bucket ? &bucket->value() : nullptr, found);
	}
}

#undef HASHMAP_TEMPLATE_TYPE
#undef HASHMAP_CLASS
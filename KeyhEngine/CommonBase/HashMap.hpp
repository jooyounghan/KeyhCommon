#define HASHMAP_TEMPLATE_TYPE template<typename Key, typename Value, typename Hasher>
#define HASHMAP_CLASS HashMap<Key, Value, Hasher>

#include <utility>

namespace keyh
{
	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::Bucket::constructBucket(int32 psl, const Key& key, const Value& value)
	{
		new (&_keyStorage) Key(key);
		new (&_valueStorage) Value(value);
		_psl = psl;
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::Bucket::constructBucket(int32 psl, Key&& key, Value&& value)
	{
		new (&_keyStorage) Key(keyh::move(key));
		new (&_valueStorage) Value(keyh::move(value));
		_psl = psl;
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::Bucket::changeBucketValue(const Value& value)
	{
		this->value().~Value();
		new (&_valueStorage) Value(value);
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::Bucket::changeBucketValue(Value&& value)
	{
		this->value().~Value();
		new (&_valueStorage) Value(keyh::move(value));
	}

	HASHMAP_TEMPLATE_TYPE
	void HASHMAP_CLASS::Bucket::swapBucket(Bucket* other)
	{
		std::swap(_keyStorage, other->_keyStorage);
		std::swap(_valueStorage, other->_valueStorage);
		std::swap(_psl, other->_psl);
	}

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
		return insertImpl(replace, keyh::move(key), keyh::move(value));
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
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::makeInsertResult(Bucket* bucket, InsertStatus status)
	{
		return InsertResult(bucket->value(), status);
	}
}

#undef HASHMAP_TEMPLATE_TYPE
#undef HASHMAP_CLASS
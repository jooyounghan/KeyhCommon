#define HASHMAP_TEMPLATE_TYPE template<typename Key, typename Value, typename Hasher>
#define HASHMAP_CLASS HashMap<Key, Value, Hasher>

#include <utility>

namespace keyh
{
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::Bucket::~Bucket()
	{
		if (!isEmpty())
		{
			key().~Key();
			value().~Value();
			_psl = HashUtil::kEmptyPsl;
		}
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::Bucket::Bucket(Bucket&& other)
	{
		if (other.isEmpty() == false)
		{
			_psl = other._psl;
			other._psl = HashUtil::kEmptyPsl;

			new (&_keyStorage) Key(keyh::move(other.key()));
			new (&_valueStorage) Value(keyh::move(other.value()));
		}
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::Bucket& HASHMAP_CLASS::Bucket::operator=(Bucket&& other)
	{
		if (this != &other && other.isEmpty() == false)
		{
			_psl = other._psl;
			other._psl = HashUtil::kEmptyPsl;

			new (&_keyStorage) Key(keyh::move(other.key()));
			new (&_valueStorage) Value(keyh::move(other.value()));
		}
		return *this;
	}


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
		Bucket tempBucket;
		tempBucket = keyh::move(*this);
		*this = keyh::move(*other);
		*other = keyh::move(tempBucket);
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
	HASHMAP_CLASS::FindResult HASHMAP_CLASS::find(const Key& key)
	{
		return findImpl(key);
	}

	HASHMAP_TEMPLATE_TYPE
	bool HASHMAP_CLASS::remove(const Key& key)
	{
		return removeImpl(key);
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult HASHMAP_CLASS::makeInsertResult(Bucket* bucket, InsertStatus status)
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
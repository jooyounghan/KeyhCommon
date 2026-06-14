#define HASHSET_TEMPLATE_TYPE template<typename Key, typename Hasher>
#define HASHSET_CLASS HashSet<Key, Hasher>

namespace keyh
{
	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::Bucket::~Bucket()
	{
		if (!isEmpty())
		{
			key().~Key();
			_psl = HashUtil::kEmptyPsl;
		}
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::Bucket::Bucket(Bucket&& other)
	{
		if (other.isEmpty() == false)
		{
			_psl = other._psl;
			other._psl = HashUtil::kEmptyPsl;

			new (&_keyStorage) Key(keyh::move(other.key()));
		}
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::Bucket& HASHSET_CLASS::Bucket::operator=(Bucket&& other)
	{
		if (this != &other && other.isEmpty() == false)
		{
			_psl = other._psl;
			other._psl = HashUtil::kEmptyPsl;

			new (&_keyStorage) Key(keyh::move(other.key()));
		}
		return *this;
	}

	HASHSET_TEMPLATE_TYPE
	void HASHSET_CLASS::Bucket::constructBucket(int32 psl, const Key& key)
	{
		new (&_keyStorage) Key(key);
		_psl = psl;
	}

	HASHSET_TEMPLATE_TYPE
	void HASHSET_CLASS::Bucket::constructBucket(int32 psl, Key&& key)
	{
		new (&_keyStorage) Key(keyh::move(key));
		_psl = psl;
	}

	HASHSET_TEMPLATE_TYPE
	void HASHSET_CLASS::Bucket::swapBucket(Bucket* other)
	{
		Bucket tempBucket;
		tempBucket = keyh::move(*this);
		*this = keyh::move(*other);
		*other = keyh::move(tempBucket);
	}


	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::~HashSet()
	{
		clear();
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::HashSet(HashSet&& other) noexcept
		: _buckets(other._buckets), _capacity(other._capacity), _size(other._size)
	{
		other._buckets = nullptr;
		other._capacity = 0;
		other._size = 0;
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS& HASHSET_CLASS::operator=(HashSet&& other) noexcept
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

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::InsertResult HASHSET_CLASS::insert(const Key& key)
	{
		return insertImpl(false, key);
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::InsertResult HASHSET_CLASS::insert(Key&& key)
	{
		return insertImpl(false, keyh::move(key));
	}

	HASHSET_TEMPLATE_TYPE
	bool HASHSET_CLASS::contains(const Key& key)
	{
		return findImpl(key).isFound();
	}

	HASHSET_TEMPLATE_TYPE
	bool HASHSET_CLASS::remove(const Key& key)
	{
		return removeImpl(key);
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::InsertResult HASHSET_CLASS::makeInsertResult(Bucket* bucket, InsertStatus status)
	{
		return InsertResult(status);
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::FindResult HASHSET_CLASS::makeFindResult(Bucket* bucket, bool found)
	{
		return FindResult(found);
	}
}

#undef HASHSET_TEMPLATE_TYPE
#undef HASHSET_CLASS
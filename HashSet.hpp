#define HASHSET_TEMPLATE_TYPE template<typename Key, typename Hasher>
#define HASHSET_CLASS HashSet<Key, Hasher>

namespace keyh
{
	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::~HashSet()
	{
		clear();
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::HashSet(HashSet&& other) noexcept
		: Base(other._capacity, other._size), _buckets(other._buckets)
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
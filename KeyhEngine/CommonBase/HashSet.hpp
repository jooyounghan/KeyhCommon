#define HASHSET_TEMPLATE_TYPE template<typename Key, typename Hasher>
#define HASHSET_CLASS HashSet<Key, Hasher>

namespace keyh
{
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
		MemoryUtil::swap(_keyStorage, other->_keyStorage);
		MemoryUtil::swap(_psl, other->_psl);
	}


	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::~HashSet()
	{
		clear();
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::HashSet(HashSet&& other) noexcept
	{

	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS& HASHSET_CLASS::operator=(HashSet&& other) noexcept
	{}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::InsertResult HASHSET_CLASS::insert(const Key& key, bool replace)
	{
		return InsertResult{ InsertStatus::Error };
	}

	HASHSET_TEMPLATE_TYPE
	HASHSET_CLASS::InsertResult HASHSET_CLASS::insert(Key&& key, bool replace)
	{
		return InsertResult{ InsertStatus::Error };
	}

	HASHSET_TEMPLATE_TYPE
	bool HASHSET_CLASS::contains(const Key& key) const
	{
		return false;
	}

	HASHSET_TEMPLATE_TYPE
	bool HASHSET_CLASS::remove(const Key& key)
	{
		return false;
	}

	HASHSET_TEMPLATE_TYPE
		HASHSET_CLASS::InsertResult HASHSET_CLASS::makeInsertResult(Bucket* bucket, InsertStatus status)
	{
		return InsertResult(bucket->key(), status);
	}
}
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
	{
	
	}
	
	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS& HASHMAP_CLASS::operator=(HashMap && other) noexcept
	{
		return *this;
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult<Key, Value> HASHMAP_CLASS::insert(const Key& key, const Value& value, bool replace)
	{
		static Value dummyValue;
		return InsertResult<Key, Value>{ dummyValue, InsertStatus::Error };
	}

	HASHMAP_TEMPLATE_TYPE
	HASHMAP_CLASS::InsertResult<Key, Value> HASHMAP_CLASS::insert(Key&& key, Value&& value, bool replace)
	{
		static Value dummyValue;
		return InsertResult<Key, Value>{ dummyValue, InsertStatus::Error };
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
}

#undef HASHMAP_TEMPLATE_TYPE
#undef HASHMAP_CLASS
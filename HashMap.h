#pragma once
#include "HashCore.h"
#include "HashContainerBase.h"

namespace keyh
{
	template<typename Key, typename Value, typename Hasher = FNV1aHash<Key>>
	class HashMap : public HashContainerBase<HashMap<Key, Value, Hasher>>
	{
	private:
		using Base = HashContainerBase<HashMap<Key, Value, Hasher>>;
		friend class HashContainerBase<HashMap<Key, Value, Hasher>>;
		
	protected:
		using Base::_capacity;
		using Base::_size;
		using Base::insertImpl;
		using Base::findImpl;
		using Base::removeImpl;

	public:
		using Base::clear;
		using Bucket = HashBucket<Key, Value>;
		using InsertResult = HashInsertResult<Value>;
		using FindResult = HashFindResult<Value>;

	public:
		HashMap() = default;
		~HashMap();

	public:
		REMOVE_COPY(HashMap);

	public:
		HashMap(HashMap&& other) noexcept;
		HashMap& operator=(HashMap&& other) noexcept;

	private:
		Bucket* _buckets = nullptr;
		Hasher	_hasher;

	public:
		// hashCache is optional and, when non-null, must point to a valid precomputed hash value for key.
		InsertResult	insert(const Key& key, const Value& value, bool replace = false, size_t* hashCache = nullptr);
		InsertResult	insert(Key&& key, Value&& value, bool replace = false, size_t* hashCache = nullptr);
		FindResult		find(const Key& key, size_t* hashCache = nullptr);
		bool			remove(const Key& key, size_t* hashCache = nullptr);

	private:
		InsertResult	makeInsertResult(Bucket* bucket, HashUtil::InsertStatus status);
		FindResult		makeFindResult(Bucket* bucket, bool found);
	};
}
#include "HashMap.hpp"
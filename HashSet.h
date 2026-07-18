#pragma once
#include "HashCore.h"
#include "HashContainerBase.h"

namespace keyh
{
	template<typename Key, typename Hasher = FNV1aHash<Key>>
	class HashSet : public HashContainerBase<HashSet<Key, Hasher>>
	{
	private:
		using Base = HashContainerBase<HashSet<Key, Hasher>>;
		friend class HashContainerBase<HashSet<Key, Hasher>>;

	protected:
		using Base::_capacity;
		using Base::_size;
		using Base::insertImpl;
		using Base::findImpl;
		using Base::removeImpl;

	public:
		using Base::clear;
		using Bucket = HashBucket<Key>;
		using InsertResult = HashInsertResult<void>;
		using FindResult = HashFindResult<const Key>;

	public:
		HashSet() = default;
		~HashSet();

	public:
		REMOVE_COPY(HashSet);

	public:
		HashSet(HashSet&& other) noexcept;
		HashSet& operator=(HashSet&& other) noexcept;

	private:
		Bucket* _buckets = nullptr;
		Hasher	_hasher;

	public:
		// hashCache is optional and, when non-null, must point to a valid precomputed hash value for key.
		InsertResult	insert(const Key& key, size_t* hashCache = nullptr);
		InsertResult	insert(Key&& key, size_t* hashCache = nullptr);
		FindResult	find(const Key& key, size_t* hashCache = nullptr);
		FindResult	find(const Key& key, size_t* hashCache = nullptr) const;
		bool			contains(const Key& key, size_t* hashCache = nullptr);
		bool			remove(const Key& key, size_t* hashCache = nullptr);

	private:
		InsertResult makeInsertResult(Bucket* bucket, HashUtil::InsertStatus status);
		FindResult makeFindResult(Bucket* bucket, bool found);
		FindResult makeFindResult(const Bucket* bucket, bool found) const;
	};
}
#include "HashSet.hpp"
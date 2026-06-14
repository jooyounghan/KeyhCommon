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
		using typename Base::InsertStatus;

	public:
		using Base::clear;
		using Bucket = HashBucket<Key>;
		using InsertResult = HashInsertResult<void, InsertStatus>;
		using FindResult = HashFindResult<void>;

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
		InsertResult	insert(const Key& key);
		InsertResult	insert(Key&& key);
		bool			contains(const Key& key);
		bool			remove(const Key& key);

	private:
		InsertResult makeInsertResult(Bucket* bucket, InsertStatus status);
		FindResult makeFindResult(Bucket* bucket, bool found);
	};
}
#include "HashSet.hpp"
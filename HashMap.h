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
		friend Base;
		
	protected:
		using Base::_capacity;
		using Base::_size;
		using Base::insertImpl;
		using Base::findImpl;
		using Base::removeImpl;
		using typename Base::InsertStatus;

	public:
		using Base::clear;
		using Bucket = HashBucket<Key, Value>;
		using InsertResult = HashInsertResult<Value, InsertStatus>;
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
		InsertResult	insert(const Key& key, const Value& value, bool replace = false);
		InsertResult	insert(Key&& key, Value&& value, bool replace = false);
		FindResult		find(const Key& key);
		bool			remove(const Key& key);

	private:
		InsertResult	makeInsertResult(Bucket* bucket, InsertStatus status);
		FindResult		makeFindResult(Bucket* bucket, bool found);
	};
}
#include "HashMap.hpp"
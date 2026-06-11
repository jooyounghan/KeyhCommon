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
		friend class Base;

	protected:
		using Base::_capacity;
		using Base::_capacityLevel;
		using Base::_size;
		using Base::insertImpl;
		using Base::clear;
		using typename Base::InsertStatus;

	private:
		struct Bucket
		{
			alignas(Key)	uint8	_keyStorage[sizeof(Key)];
			int32					_psl;

			bool isEmpty() const noexcept { return _psl < 0; }
			Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
			const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }
		};

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

	private:
		struct InsertResult
		{
		public:
			InsertResult(InsertStatus success)
				: _success(success) {}

		private:
			InsertStatus	_success;

		public:
			bool isSuccess() const noexcept { return _success == InsertStatus::Inserted || _success == InsertStatus::Replaced; }
			bool isDenied() const noexcept { return _success == InsertStatus::AlreadyExists; }
			bool isError() const noexcept { return _success == InsertStatus::Error; }
		};

	public:
		InsertResult	insert(const Key& key, bool replace = false);
		InsertResult	insert(Key&& key, bool replace = false);
		bool			contains(const Key& key) const;
		bool			remove(const Key& key);

	private:
		void constructBucket(Bucket& bucket, int32 psl, const Key& key);
		void constructBucket(Bucket& bucket, int32 psl, Key&& key);
		InsertResult makeInsertResult(Bucket& bucket, InsertStatus status);
	};
}
#include "HashSet.hpp"
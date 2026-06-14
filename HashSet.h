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
		using Base::_size;
		using Base::insertImpl;
		using Base::findImpl;
		using Base::removeImpl;
		using typename Base::InsertStatus;

	public:
		using Base::clear;

	private:
		class Bucket
		{
		public:
			Bucket() = default;
			~Bucket();

		public:
			Bucket(const Bucket&) = delete;
			Bucket& operator=(const Bucket&) = delete;
			Bucket(Bucket&&);
			Bucket& operator=(Bucket&&);

		private:
			alignas(Key)	uint8	_keyStorage[sizeof(Key)];
			int32					_psl = HashUtil::kEmptyPsl;

		public:
			bool isEmpty() const noexcept { return _psl < 0; }
			inline int32 getPsl() const noexcept { return _psl; }
			inline void setPsl(int32 psl) noexcept { _psl = psl; }
			
		public:
			Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
			const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }

		public:
			void constructBucket(int32 psl, const Key& key);
			void constructBucket(int32 psl, Key&& key);
			void changeBucketValue() { __noop; }

		public:
			void swapBucket(Bucket* other);
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
		class InsertResult
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

		class FindResult
		{
		public:
			FindResult(bool found)
				: _found(found) {}

		private:
			bool _found;

		public:
			bool isFound() const noexcept { return _found; }
		};

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
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
			alignas(Value)	uint8	_valueStorage[sizeof(Value)];
			int32					_psl = HashUtil::kEmptyPsl;

		public:
			bool isEmpty() const noexcept { return _psl < 0; }
			inline int32 getPsl() const noexcept { return _psl; }
			inline void setPsl(int32 psl) noexcept { _psl = psl; }

		public:
			Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
			const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }
			Value& value() noexcept { return *reinterpret_cast<Value*>(_valueStorage); }
			const Value& value() const noexcept { return *reinterpret_cast<const Value*>(_valueStorage); }

		public:
			void constructBucket(int32 psl, const Key& key, const Value& value);
			void constructBucket(int32 psl, Key&& key, Value&& value);
			void changeBucketValue(const Value& value);
			void changeBucketValue(Value&& value);

		public:
			void swapBucket(Bucket* other);
		};

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

	private:
		class InsertResult
		{
		public:
			InsertResult(Value& value, InsertStatus success)
				: _value(value), _success(success) {}

		public:
			Value&			_value;

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
			FindResult(Value* value, bool found)
				: _value(found ? value : nullptr), _found(found) {}

		private:
			Value* _value;
			bool	_found;

		public:
			Value* value() noexcept { return _value; }
			const Value* value() const noexcept { return _value; }
			bool isFound() const noexcept { return _found; }
		};

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
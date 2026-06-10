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
		using Base::_capacityLevel;
		using Base::_size;
		using Base::insertImpl;

	private:
		struct Bucket
		{
			alignas(Key)	uint8	_keyStorage[sizeof(Key)];
			alignas(Value)	uint8	_valueStorage[sizeof(Value)];
			int32					_psl;

			bool isEmpty() const noexcept { return _psl < 0; }
			Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
			const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }
			Value& value() noexcept { return *reinterpret_cast<Value*>(_valueStorage); }
			const Value& value() const noexcept { return *reinterpret_cast<const Value*>(_valueStorage); }
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
		enum class InsertStatus
		{
			Inserted,
			Replaced,
			AlreadyExists,
			Error
		};

		struct InsertResultValue
		{
		public:
			InsertResultValue(Value& value, InsertStatus success)
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

		using InsertResult = InsertResultValue;

	public:
		InsertResult	insert(const Key& key, const Value& value, bool replace = false);
		InsertResult	insert(Key&& key, Value&& value, bool replace = false);
		Value*			find(const Key& key);
		const Value*	find(const Key& key) const;
		bool			remove(const Key& key);
		void			clear();

	private:
		void constructBucket(Bucket& bucket, const Key& key, const Value& value)
		{
			new (&bucket._keyStorage) Key(key);
			new (&bucket._valueStorage) Value(value);
		}

		void constructBucket(Bucket& bucket, Key&& key, Value&& value)
		{
			new (&bucket._keyStorage) Key(move(key));
			new (&bucket._valueStorage) Value(smove(value));
		}

		InsertResult makeInsertResult(Bucket& bucket, InsertStatus status)
		{
			return InsertResult{ bucket.value(), status };
		}

	private:
		void rehash(size_t newCapacity);
	};
}
#include "HashMap.hpp"
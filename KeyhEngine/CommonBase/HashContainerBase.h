#pragma once
#include "HashUtil.h"
#include "CircularBufferUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	template<typename InsertStatusT>
	class HashInsertResultBase
	{
	protected:
		explicit HashInsertResultBase(InsertStatusT status)
			: _status(status) {}

	private:
		InsertStatusT _status;

	public:
		bool isSuccess() const noexcept { return _status == InsertStatusT::Inserted || _status == InsertStatusT::Replaced; }
		bool isDenied() const noexcept { return _status == InsertStatusT::AlreadyExists; }
		bool isError() const noexcept { return _status == InsertStatusT::Error; }
	};

	template<typename Value, typename InsertStatusT>
	class HashInsertResult : public HashInsertResultBase<InsertStatusT>
	{
	private:
		using Base = HashInsertResultBase<InsertStatusT>;

	public:
		HashInsertResult(Value& value, InsertStatusT status)
			: Base(status), _value(value) {}

	public:
		Value& _value;
	};

	template<typename InsertStatusT>
	class HashInsertResult<void, InsertStatusT> : public HashInsertResultBase<InsertStatusT>
	{
	private:
		using Base = HashInsertResultBase<InsertStatusT>;

	public:
		explicit HashInsertResult(InsertStatusT status)
			: Base(status) {}
	};

	template<typename Value>
	class HashFindResult
	{
	public:
		HashFindResult(Value* value, bool found)
			: _value(found ? value : nullptr), _found(found) {}

	private:
		Value* _value;
		bool	_found;

	public:
		Value* value() noexcept { return _value; }
		const Value* value() const noexcept { return _value; }
		bool isFound() const noexcept { return _found; }
	};

	template<>
	class HashFindResult<void>
	{
	public:
		explicit HashFindResult(bool found)
			: _found(found) {}

	private:
		bool _found;

	public:
		bool isFound() const noexcept { return _found; }
	};

	template<typename Key, typename Value = void>
	class HashBucket;

	template<typename Key>
	class HashBucket<Key, void>
	{
	public:
		HashBucket() = default;
		~HashBucket() { destroy(); }

	public:
		HashBucket(const HashBucket&) = delete;
		HashBucket& operator=(const HashBucket&) = delete;
		HashBucket(HashBucket&& other)
		{
			moveFrom(other);
		}
		HashBucket& operator=(HashBucket&& other)
		{
			if (this != &other)
			{
				destroy();
				moveFrom(other);
			}
			return *this;
		}

	private:
		alignas(Key)	uint8	_keyStorage[sizeof(Key)];
		int32					_psl = HashUtil::kEmptyPsl;

	public:
		bool isEmpty() const noexcept { return _psl < 0; }
		int32 getPsl() const noexcept { return _psl; }
		void setPsl(int32 psl) noexcept { _psl = psl; }

	public:
		Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
		const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }

	public:
		void constructBucket(int32 psl, const Key& keyValue)
		{
			destroy();
			new (&_keyStorage) Key(keyValue);
			_psl = psl;
		}

		void constructBucket(int32 psl, Key&& keyValue)
		{
			destroy();
			new (&_keyStorage) Key(keyh::move(keyValue));
			_psl = psl;
		}

		void changeBucketValue() noexcept {}

	public:
		void swapBucket(HashBucket* other)
		{
			if (this == other)
				return;

			HashBucket tempBucket(keyh::move(*this));
			*this = keyh::move(*other);
			*other = keyh::move(tempBucket);
		}

	private:
		void destroy()
		{
			if (!isEmpty())
			{
				key().~Key();
				_psl = HashUtil::kEmptyPsl;
			}
		}

		void moveFrom(HashBucket& other)
		{
			if (other.isEmpty())
			{
				_psl = HashUtil::kEmptyPsl;
				return;
			}

			new (&_keyStorage) Key(keyh::move(other.key()));
			_psl = other._psl;
			other.destroy();
		}
	};

	template<typename Key, typename Value>
	class HashBucket
	{
	public:
		HashBucket() = default;
		~HashBucket() { destroy(); }

	public:
		HashBucket(const HashBucket&) = delete;
		HashBucket& operator=(const HashBucket&) = delete;
		HashBucket(HashBucket&& other)
		{
			moveFrom(other);
		}
		HashBucket& operator=(HashBucket&& other)
		{
			if (this != &other)
			{
				destroy();
				moveFrom(other);
			}
			return *this;
		}

	private:
		alignas(Key)	uint8	_keyStorage[sizeof(Key)];
		alignas(Value)	uint8	_valueStorage[sizeof(Value)];
		int32					_psl = HashUtil::kEmptyPsl;

	public:
		bool isEmpty() const noexcept { return _psl < 0; }
		int32 getPsl() const noexcept { return _psl; }
		void setPsl(int32 psl) noexcept { _psl = psl; }

	public:
		Key& key() noexcept { return *reinterpret_cast<Key*>(_keyStorage); }
		const Key& key() const noexcept { return *reinterpret_cast<const Key*>(_keyStorage); }
		Value& value() noexcept { return *reinterpret_cast<Value*>(_valueStorage); }
		const Value& value() const noexcept { return *reinterpret_cast<const Value*>(_valueStorage); }

	public:
		void constructBucket(int32 psl, const Key& keyValue, const Value& valueData)
		{
			destroy();
			new (&_keyStorage) Key(keyValue);
			new (&_valueStorage) Value(valueData);
			_psl = psl;
		}

		void constructBucket(int32 psl, Key&& keyValue, Value&& valueData)
		{
			destroy();
			new (&_keyStorage) Key(keyh::move(keyValue));
			new (&_valueStorage) Value(keyh::move(valueData));
			_psl = psl;
		}

		void changeBucketValue(const Value& valueData)
		{
			value().~Value();
			new (&_valueStorage) Value(valueData);
		}

		void changeBucketValue(Value&& valueData)
		{
			value().~Value();
			new (&_valueStorage) Value(keyh::move(valueData));
		}

	public:
		void swapBucket(HashBucket* other)
		{
			if (this == other)
				return;

			HashBucket tempBucket(keyh::move(*this));
			*this = keyh::move(*other);
			*other = keyh::move(tempBucket);
		}

	private:
		void destroy()
		{
			if (!isEmpty())
			{
				key().~Key();
				value().~Value();
				_psl = HashUtil::kEmptyPsl;
			}
		}

		void moveFrom(HashBucket& other)
		{
			if (other.isEmpty())
			{
				_psl = HashUtil::kEmptyPsl;
				return;
			}

			new (&_keyStorage) Key(keyh::move(other.key()));
			new (&_valueStorage) Value(keyh::move(other.value()));
			_psl = other._psl;
			other.destroy();
		}
	};

	template <typename Derived>
	class HashContainerBase
	{
	protected:
		enum class InsertStatus
		{
			Inserted,
			Replaced,
			AlreadyExists,
			Error
		};

	protected:
		size_t _capacity = 0;
		size_t _size = 0;

	protected:
		HashContainerBase() = default;
		HashContainerBase(size_t capacity, size_t size) noexcept
			: _capacity(capacity), _size(size) {}

	protected:
		template <typename Key, typename... Args>
		auto insertImpl(bool replace, Key&& key, Args&&... args);

	private:
		template<typename BucketT>
		auto insertBucket(BucketT&& bucket);

	protected:
		template<typename Key>
		bool removeImpl(const Key& key);

	protected:
		template<typename Key>
		auto findImpl(const Key& key);

	protected:
		void rehashIfNeeded();
		void rehash(size_t newCapacity);

	public:
		void reserve(size_t newCapacity);
		void clear();
	};
}
#include "HashContainerBase.hpp"

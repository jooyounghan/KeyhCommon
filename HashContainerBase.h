#pragma once

#include "HashUtil.h"
#include "CircularBufferUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	using InsertStatus = HashUtil::InsertStatus;

	class HashInsertResultBase
	{
	protected:
		explicit HashInsertResultBase(InsertStatus status)
			: _status(status) {}

	private:
		InsertStatus _status;

	public:
		bool isSuccess() const noexcept { return _status == InsertStatus::Inserted || _status == InsertStatus::Replaced; }
		bool isDenied() const noexcept { return _status == InsertStatus::AlreadyExists; }
		bool isError() const noexcept { return _status == InsertStatus::Error; }
	};

	template<typename Value>
	class HashInsertResult : public HashInsertResultBase
	{
	private:
		using Base = HashInsertResultBase;

	public:
		HashInsertResult(Value& value, InsertStatus status)
			: Base(status), _value(value) {}

	private:
		Value& _value;

	public:
		inline Value& value() { return _value; }
		inline const Value& value() const { return _value; }
	};

	template<>
	class HashInsertResult<void> : public HashInsertResultBase
	{
	private:
		using Base = HashInsertResultBase;

	public:
		explicit HashInsertResult(InsertStatus status)
			: Base(status) {}
	};

	template<typename Value>
	class HashFindResult
	{
	public:
		HashFindResult(Value* value, bool found)
			: _value(found ? value : nullptr), _found(found) {}

	private:
		Value*	_value;
		bool	_found;

	public:
		Value* value() noexcept { return _value; }
		const Value* value() const noexcept { return _value; }
		bool isFound() const noexcept { return _found; }
	};

	template<typename Value>
	struct HashConstFindValue
	{
		using type = const Value;
	};

	template<typename Value>
	struct HashConstFindValue<Value*>
	{
		using type = const Value*;
	};

	template<typename Value>
	using HashConstFindValue_t = typename HashConstFindValue<Value>::type;

	class HashBucketBase
	{
	protected:
		HashBucketBase() = default;
		~HashBucketBase() = default;

	protected:
		int32 _psl = HashUtil::kEmptyPsl;

	public:
		bool isEmpty() const noexcept { return _psl < 0; }
		int32 getPsl() const noexcept { return _psl; }
		void setPsl(int32 psl) noexcept { _psl = psl; }
	};

	template<typename Key, typename Value = void>
	class HashBucket;

	template<typename Key>
	class HashBucket<Key, void> : public HashBucketBase
	{
	private:
		using Base = HashBucketBase;

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
	class HashBucket : public HashBucketBase
	{
	private:
		using Base = HashBucketBase;

	public:
		HashBucket() = default;
		~HashBucket() { destroy(); }

	public:
		HashBucket(const HashBucket&) = delete;
		HashBucket& operator=(const HashBucket&) = delete;
		HashBucket(HashBucket&& other) noexcept
		{
			moveFrom(other);
		}
		HashBucket& operator=(HashBucket&& other) noexcept
		{
			if (this != &other)
			{
				destroy();
				moveFrom(other);
			}
			return *this;
		}

	private:
		alignas(Key)	uint8	_keyStorage[sizeof(Key)]{};
		alignas(Value)	uint8	_valueStorage[sizeof(Value)]{};

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
	public:
		class Iterator
		{
		public:
			using Bucket = typename Derived::Bucket;

			Iterator(Bucket* current, Bucket* end);

		public:
			inline Bucket& operator*() { KEYH_ASSERT(_current != _end, "HashContainerBase::Iterator: dereferencing end iterator"); return *_current; }
			inline Bucket* operator->() { KEYH_ASSERT(_current != _end, "HashContainerBase::Iterator: dereferencing end iterator"); return _current; }
			inline bool operator==(const Iterator& other) const { return _current == other._current; }
			inline bool operator!=(const Iterator& other) const { return _current != other._current; }
			Iterator& operator++();
			Iterator operator++(int);

		private:
			Bucket* _current;
			Bucket* _end;
		};

		class ConstIterator
		{
		public:
			using Bucket = typename Derived::Bucket;

			ConstIterator(const Bucket* current, const Bucket* end);

		public:
			inline const Bucket& operator*() const { KEYH_ASSERT(_current != _end, "HashContainerBase::ConstIterator: dereferencing end iterator"); return *_current; }
			inline const Bucket* operator->() const { KEYH_ASSERT(_current != _end, "HashContainerBase::ConstIterator: dereferencing end iterator"); return _current; }
			inline bool operator==(const ConstIterator& other) const { return _current == other._current; }
			inline bool operator!=(const ConstIterator& other) const { return _current != other._current; }
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const Bucket* _current;
			const Bucket* _end;
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
		auto insertImpl(bool replace, size_t* hashCache, Key&& key, Args&&... args);

	private:
		template<typename BucketT>
		auto insertBucket(BucketT&& bucket);

	protected:
		template<typename Key>
		bool removeImpl(const Key& key, size_t* hashCache = nullptr);

	protected:
		template<typename Key>
		auto findImpl(const Key& key, size_t* hashCache = nullptr);

		template<typename Key>
		auto findImpl(const Key& key, size_t* hashCache = nullptr) const;

	private:
		template<typename Self, typename Key>
		static auto findImplInternal(Self* self, const Key& key, size_t* hashCache);

	protected:
		void rehashIfNeeded();
		void rehash(size_t newCapacity);

	public:
		void reserve(size_t newCapacity);
		void clear();

	public:
		inline size_t capacity() const noexcept { return _capacity; }
		inline size_t size() const noexcept { return _size; }
		inline bool empty() const noexcept { return _size == 0; }

	public:
		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
	};
}

#include "HashContainerBase.hpp"
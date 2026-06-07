#pragma once
#include "HashContainerCommon.h"

namespace keyh
{
	template<typename Key, typename Hasher = Hash<Key>>
	class HashSet
	{
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
		size_t	_capacity = 0;
		size_t	_size = 0;
		Hasher	_hasher;

	private:
		enum class InsertStatus
		{
			Inserted,
			Replaced,
			AlreadyExists,
			Error
		};

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
		void			clear();
	};
}
#include "HashSet.hpp"
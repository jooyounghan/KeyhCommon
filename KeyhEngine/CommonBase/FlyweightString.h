#pragma once
#include "CommonCore.h"

#include "StaticString.h"
#include "StringView.h"
#include "HashMap.h"
#include "StaticArray.h"

namespace keyh
{
	template<typename T, size_t PoolSize = 65536>
	class StringPool
	{
	public:
		SINGLETON(StringPool);
		~StringPool() = default;

	private:
		struct StringOffset
		{
			uint32 _offsetBegin;
			uint32 _offsetEnd;
		};

	private:
		StaticArray<T, PoolSize>				_stringContainer;
		HashMap<StringView<T>, StringOffset>	_stringOffsets;
		size_t									_currentOffset = 0;

	public:
		StringView<T> findOrInsert(const StringView<T>& str, size_t hash);
	};

	template<typename T, typename Hasher = FNV1aHash<StringView<T>>>
	class FlyweightString
	{
	public:
		static const FlyweightString Empty;

	public:
		FlyweightString() = default;
		~FlyweightString() = default;

	public:
		FlyweightString(const T* str);
		FlyweightString(const StaticString<T>& str);
		FlyweightString(const StringView<T>& str);

	private:
		size_t _hash = HashUtil::kInvalidHash;
		Hasher _hasher;

	private:
		StringView<T> _stringView;
		
	public:
		bool operator==(const FlyweightString& other) const;
		bool operator!=(const FlyweightString& other) const;

	public:
		inline size_t getHash() const { return _hash; }
		inline const StringView<T>& getStringView() const { return _stringView; }
		inline const T* c_str() const { return _stringView.c_str(); }
		inline size_t length() const { return _stringView.length(); }
		inline size_t size() const { return _stringView.size(); }
		inline bool empty() const { return _stringView.empty(); }
	};

	using FlyweightStringA = FlyweightString<char, FNV1aHash<StringView<char>>>;
	using FlyweightStringW = FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>>;
}

#pragma region FNV1aHash Specializations
#include "FNV1aHasher.h"
namespace keyh
{
	DECLARE_TEMPLATED_FNV1A_HASH_SPECIALIZATION(FlyweightString, char);
	DECLARE_TEMPLATED_FNV1A_HASH_SPECIALIZATION(FlyweightString, wchar_t);
}
#pragma endregion
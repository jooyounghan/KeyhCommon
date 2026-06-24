#pragma once
#include "CommonCore.h"

#include "StaticString.h"
#include "StringView.h"
#include "HashMap.h"
#include "HashSet.h"
#include "Vector.h"
#include "StaticArray.h"
#include "Ptr.h"

namespace keyh
{
	template<typename T>
	class StringPool1
	{
	public:
		SINGLETON(StringPool1);
		~StringPool1() = default;

	private:
		HashSet<StaticString<T>>	_stringContainer;

	public:
		StringView<T> findOrInsert(const StringView<T>& str, size_t hash);
	};

	template<typename T, size_t PoolSize = 65536>
	class StringPool2
	{
	public:
		SINGLETON(StringPool2);
		~StringPool2() = default;

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

	template<typename T, typename Hasher = FNV1aHash<StringView<T>>, typename StringPool = StringPool1<T>>
	class FlyweightString
	{
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
		inline size_t getHash() const { return _hash; }
		inline const StringView<T>& getStringView() const { return _stringView; }
		inline const T* c_str() const { return _stringView.c_str(); }
		inline size_t length() const { return _stringView.length(); }
		inline size_t size() const { return _stringView.size(); }
		inline bool empty() const { return _stringView.empty(); }
	};
}
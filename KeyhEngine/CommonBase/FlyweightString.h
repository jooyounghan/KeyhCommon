#pragma once
#include "StaticString.h"
#include "StringView.h"
#include "HashMap.h"
#include "HashSet.h"
#include "Vector.h"
#include "StaticArray.h"
#include "Ptr.h"

namespace keyh
{
	template<typename T, typename Hasher = FNV1aHash<T>>
	class StringPool1
	{
	public:
		SINGLETON(StringPool1);
		~StringPool1() = default;

	private:
		HashSet<StaticString<T>>	_stringContainer;
		Hasher _hasher;

	public:
		StringView<T> findOrInsert(const StringView<T>& str);
	};

	template<typename T, typename Hasher = FNV1aHash<T>, size_t PoolSize = 4096>
	class StringPool2
	{
	public:
		SINGLETON(StringPool2);
		~StringPool2() = default;

	private:
		StaticArray<T, PoolSize>			_stringContainer;
		Vector<uint32>						_stringOffsets;
		Hasher _hasher;

	public:
		StringView<T> findOrInsert(const StringView<T>& str);
	};

	template<typename T, typename Hasher = FNV1aHash<T>, size_t PoolSize = 4096>
	class StringPool3
	{
	public:
		SINGLETON(StringPool3);
		~StringPool3() = default;

	private:
		StaticArray<T, PoolSize>			_stringContainer;
		HashMap<StringView<T>, uint32>		_stringOffsets;
		Hasher _hasher;

	public:
		StringView<T> findOrInsert(const StringView<T>& str);
	};

	template<typename T, typename Hasher = FNV1aHash<T>, typename StringPool = StringPool1<T, Hasher>>
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
		Hasher _hasher;

	private:
		StringView<T> _stringView;

	public:
		inline const T* c_str() const { return _stringView.c_str(); }
		inline size_t length() const { return _stringView.length(); }
		inline size_t size() const { return _stringView.size(); }
		inline bool empty() const { return _stringView.empty(); }
	};
}
#include "FlyweightString.hpp"
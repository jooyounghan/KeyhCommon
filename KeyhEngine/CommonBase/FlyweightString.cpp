#include "CommonBasePch.h"
#include "FlyweightString.h"

namespace keyh
{
	template<typename T>
	StringView<T> StringPool1<T>::findOrInsert(const StringView<T>& str, size_t hash)
	{
		StaticString<T> staticStr(str.c_str(), str.length());
		typename HashSet<StaticString<T>>::InsertResult insertResult = _stringContainer.insert(staticStr, &hash);
		const StaticString<T>& insertedKey = insertResult._value;
		return StringView<T>(insertedKey.c_str(), insertedKey.length());
	}

	template<typename T, size_t PoolSize>
	StringView<T> StringPool2<T, PoolSize>::findOrInsert(const StringView<T>& str, size_t hash)
	{
		typename HashMap<StringView<T>, StringOffset>::FindResult findResult = _stringOffsets.find(str, &hash);
		if (findResult.isFound())
		{
			const StringOffset& offset = findResult.value();
			const T* existingStr = _stringContainer.data() + offset._offsetBegin;
			return StringView<T>(existingStr, offset._offsetEnd - offset._offsetBegin);
		}
		else
		{
			memcpy(_stringContainer.data() + _currentOffset, str.data(), str.length() * sizeof(T));
			StringOffset offset = { static_cast<uint32>(_currentOffset), static_cast<uint32>(_currentOffset + str.length()) };
			_stringOffsets.insert(str, offset, &hash);
			_currentOffset += str.length();
			return StringView<T>(_stringContainer.data() + offset._offsetBegin, offset._offsetEnd - offset._offsetBegin);
		}
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const T* str)
	{
		StringView<T> stringView(str);
		_hash = _hasher(stringView);
		_stringView = StringPool::getInstance().findOrInsert(stringView, _hash);
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const StaticString<T>& str)
	{
		StringView<T> stringView(str.c_str(), str.length());
		_hash = _hasher(stringView);
		_stringView = StringPool::getInstance().findOrInsert(stringView, _hash);
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const StringView<T>& str)
	{
		_hash = _hasher(str);
		_stringView = StringPool::getInstance().findOrInsert(str, _hash);
	}


	template class StringPool2<char>;
	template class StringPool2<wchar_t>;

	template class FlyweightString<char>;
	template class FlyweightString<wchar_t>;
	template class FlyweightString<char, FNV1aHash<StringView<char>>, StringPool2<char>>;
	template class FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>, StringPool2<wchar_t>>;

	using FlyweightStringA = FlyweightString<char>;
	using FlyweightStringW = FlyweightString<wchar_t>;
	using FlyweightStringA2 = FlyweightString<char, FNV1aHash<StringView<char>>, StringPool2<char>>;
	using FlyweightStringW2 = FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>, StringPool2<wchar_t>>;
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	template<>
	size_t FNV1aHash<FlyweightString<char>>::operator()(const FlyweightString<char>& value) const noexcept
	{
		//return value.getHash();
		return FNV1aHasher::hash(value.getStringView());
	}

	template<>
	size_t FNV1aHash<FlyweightString<wchar_t>>::operator()(const FlyweightString<wchar_t>& value) const noexcept
	{
		//return value.getHash();
		return FNV1aHasher::hash(value.getStringView());
	}
}
#pragma endregion}
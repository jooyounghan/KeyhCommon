#include "CommonBasePch.h"
#include "FlyweightString.h"

namespace keyh
{
	template<typename T, size_t PoolSize>
	StringView<T> StringPool<T, PoolSize>::findOrInsert(const StringView<T>& str, size_t hash)
	{
		typename HashMap<StringView<T>, StringOffset>::FindResult findResult = _stringOffsets.find(str, &hash);
		if (findResult.isFound())
		{
			const StringOffset* offset = findResult.value();
			const T* existingStr = _stringContainer.begin() + offset->_offsetBegin;
			return StringView<T>(existingStr, offset->_offsetEnd - offset->_offsetBegin);
		}
		else
		{
			const size_t needed = str.length() + 1;
			const size_t available = PoolSize - _currentOffset;
			KEYH_ASSERT_ARGS(needed <= available, "StringPool buffer overflow: need %zu elements but only %zu available (PoolSize=%zu)", needed, available, static_cast<size_t>(PoolSize));

			memcpy(_stringContainer.begin() + _currentOffset, str.data(), str.length() * sizeof(T));
			_stringContainer[_currentOffset + str.length()] = T();
			StringOffset offset = { static_cast<uint32>(_currentOffset), static_cast<uint32>(_currentOffset + str.length()) };
			StringView<T> pooledView(_stringContainer.begin() + offset._offsetBegin, offset._offsetEnd - offset._offsetBegin);
			_stringOffsets.insert(pooledView, offset, &hash);
			_currentOffset += str.length() + 1;
			return pooledView;
		}
	}

	template<typename T, typename Hasher>
	FlyweightString<T, Hasher>::FlyweightString(const T* str)
	{
		StringView<T> stringView(str);
		_hash = _hasher(stringView);
		_stringView = StringPool<T>::getInstance().findOrInsert(stringView, _hash);
	}

	template<typename T, typename Hasher>
	FlyweightString<T, Hasher>::FlyweightString(const StaticString<T>& str)
	{
		StringView<T> stringView(str.c_str(), str.length());
		_hash = _hasher(stringView);
		_stringView = StringPool<T>::getInstance().findOrInsert(stringView, _hash);
	}

	template<typename T, typename Hasher>
	FlyweightString<T, Hasher>::FlyweightString(const StringView<T>& str)
	{
		_hash = _hasher(str);
		_stringView = StringPool<T>::getInstance().findOrInsert(str, _hash);
	}

	template class FlyweightString<char>;
	template class FlyweightString<wchar_t>;
	template class FlyweightString<char, FNV1aHash<StringView<char>>>;
	template class FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>>;

	using FlyweightStringA = FlyweightString<char, FNV1aHash<StringView<char>>>;
	using FlyweightStringW = FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>>;
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
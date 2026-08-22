#include "CommonBasePch.h"
#include "StringView.h"

#include <cstring>

namespace keyh
{
	template<typename T>
	bool StringView<T>::operator==(const StringView& other) const
	{
		KEYH_ASSERT(_data != nullptr, "StringView data is null");

		if (_length != other._length)
			return false;

		if (_data == other._data)
			return true;

		return std::memcmp(_data, other._data, _length * sizeof(T)) == 0;
	}

	template<typename T>
	bool StringView<T>::operator!=(const StringView& other) const
	{
		return !(*this == other);
	}

	template<typename T>
	const T& StringView<T>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _length, "Index out of bounds");
		return _data[index];
	}

	template class StringView<char>;
	template class StringView<wchar_t>;
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	template<>
	size_t FNV1aHash<StringView<char>>::operator()(const StringView<char>& value) const noexcept
	{
		return FNV1aHasher::hash(value.c_str(), value.length());
	}

	template<>
	size_t FNV1aHash<StringView<wchar_t>>::operator()(const StringView<wchar_t>& value) const noexcept
	{
		return FNV1aHasher::hash(value.c_str(), value.length());
	}
}
#pragma endregion
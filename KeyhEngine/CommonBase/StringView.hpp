#include "StringView.h"
namespace keyh
{
	template<typename T>
	bool StringView<T>::operator==(const StringView& other) const
	{
		KEYH_ASSERT(_data != nullptr, "StringView data is null");

		if (_data == other._data)
			return true;

		if (_length != other._length)
			return false;

		return StrUtil::strcmp(_data, other._data) == 0;
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
}
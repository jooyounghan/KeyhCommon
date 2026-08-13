#include "CommonBasePch.h"
#include "StaticString.h"
#include "MemoryUtil.h"

namespace keyh
{
	template<typename T>
	T StaticString<T>::gNullChar = T();

	template<typename T>
	StaticString<T>::StaticString() : _stringInfo(0)
	{
		memset(_ssoBuffer, 0, sizeof(_ssoBuffer));
	}

	template<typename T>
	StaticString<T>::~StaticString()
	{
		clear();
	}

	template<typename T>
	StaticString<T>::StaticString(const T* str) : _stringInfo(0)
	{
		const size_t length = StrUtil::strlen(str);
		setLength(length);

		const bool ssoEnabled = length < StrUtil::kSsoCapacity;
		setSso(ssoEnabled);

		if (ssoEnabled == false)
		{
			_heap = new T[length + 1];
		}

		T* dstBuffer = ssoEnabled ? _ssoBuffer : _heap;
		std::memcpy(dstBuffer, str, length * sizeof(T));
		dstBuffer[length] = T();
	}

	template<typename T>
	StaticString<T>::StaticString(const T* str, size_t length) : _stringInfo(0)
	{
		setLength(length);

		const bool ssoEnabled = length < StrUtil::kSsoCapacity;
		setSso(ssoEnabled);

		if (ssoEnabled == false)
		{
			_heap = new T[length + 1];
		}

		T* dstBuffer = ssoEnabled ? _ssoBuffer : _heap;
		std::memcpy(dstBuffer, str, length * sizeof(T));
		dstBuffer[length] = T();
	}

	template<typename T>
	StaticString<T>::StaticString(const StaticString& other) : _stringInfo(0)
	{
		_stringInfo = other._stringInfo;
		const bool srcIsSso = other.isSso();
		const size_t srcLength = other.length();

		if (srcIsSso == false)
		{
			_heap = new T[srcLength + 1];
		}
		T* dstBuffer = srcIsSso ? _ssoBuffer : _heap;
		const T* srcBuffer = srcIsSso ? other._ssoBuffer : other._heap;

		std::memcpy(dstBuffer, srcBuffer, srcLength * sizeof(T));
		dstBuffer[srcLength] = T();
	}

	template<typename T>
	StaticString<T>& StaticString<T>::operator=(const StaticString& other)
	{
		if (this != &other)
		{
			StaticString temp(other);
			this->swap(temp);
		}
		return *this;
	}

	template<typename T>
	StaticString<T>::StaticString(StaticString&& other) noexcept : _stringInfo(0)
	{
		_stringInfo = other._stringInfo;
		other._stringInfo = 0;

		if (isSso())
		{
			std::memcpy(_ssoBuffer, other._ssoBuffer, StrUtil::kSsoCapacity * sizeof(T));
		}
		else
		{
			_heap = other._heap;
			other._heap = nullptr;
		}
	}

	template<typename T>
	StaticString<T>& StaticString<T>::operator=(StaticString&& other) noexcept
	{
		if (this != &other)
		{
			StaticString temp(keyh::move(other));
			this->swap(temp);
		}
		return *this;
	}

	template<typename T>
	bool StaticString<T>::operator==(const StaticString& other) const
	{
		if (_stringInfo != other._stringInfo)
			return false;

		const T* lhsBuffer = isSso() ? _ssoBuffer : _heap;
		const T* rhsBuffer = other.isSso() ? other._ssoBuffer : other._heap;
		return StrUtil::strcmp(lhsBuffer, rhsBuffer) == 0;
	}

	template<typename T>
	bool StaticString<T>::operator!=(const StaticString& other) const
	{
		return (*this == other) == false;
	}

	template<typename T>
	T& StaticString<T>::operator[](size_t index)
	{
		if (index >= getLength())
		{
			KEYH_ASSERT(false, "Index out of bounds");
			return gNullChar;
		}

		T* buffer = isSso() ? _ssoBuffer : _heap;
		return buffer[index];
	}

	template<typename T>
	const T& StaticString<T>::operator[](size_t index) const
	{
		return const_cast<StaticString<T>*>(this)->operator[](index);
	}

	template<typename T>
	void StaticString<T>::swap(StaticString& other) noexcept
	{
		MemoryUtil::swap(_stringInfo, other._stringInfo);

		char tempBytes[sizeof(_ssoBuffer)];
		std::memcpy(tempBytes, &_ssoBuffer, sizeof(_ssoBuffer));
		std::memcpy(&_ssoBuffer, &other._ssoBuffer, sizeof(_ssoBuffer));
		std::memcpy(&other._ssoBuffer, tempBytes, sizeof(_ssoBuffer));
	}

	template<typename T>
	void StaticString<T>::clear()
	{
		if (getLength() > 0 && isSso() == false)
		{
			delete[] _heap;
			_heap = nullptr;
		}
		_stringInfo = 0;
	}

	template class StaticString<char>;
	template class StaticString<wchar_t>;
}

#pragma region FNV1aHash Specializations
namespace keyh
{
	template<>
	size_t FNV1aHash<StaticString<char>>::operator()(const StaticString<char>& value) const noexcept
	{
		return FNV1aHasher::hash(value.c_str(), value.length());
	}

	template<>
	size_t FNV1aHash<StaticString<wchar_t>>::operator()(const StaticString<wchar_t>& value) const noexcept
	{
		return FNV1aHasher::hash(value.c_str(), value.length());
	}
}
#pragma endregion
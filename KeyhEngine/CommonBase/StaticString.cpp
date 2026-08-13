#include "CommonBasePch.h"
#include "StaticString.h"
#include "MemoryUtil.h"

namespace keyh
{
	template<typename T>
	T StaticString<T>::gNullChar = T();

	template<typename T>
	StaticString<T>::StaticString() : _size(0), _capacityInfo(0)
	{
		memset(_ssoBuffer, 0, sizeof(_ssoBuffer));
	}

	template<typename T>
	StaticString<T>::~StaticString()
	{
		clear();
	}

	template<typename T>
	StaticString<T>::StaticString(const T* str) : _size(0), _capacityInfo(0)
	{
		const size_t length = StrUtil::strlen(str);
		_size = length;

		const bool ssoEnabled = length < StrUtil::kSsoCapacity;
		setHeapAllocated(!ssoEnabled);

		if (ssoEnabled == false)
		{
			const size_t heapCapacity = length + 1;
			_heap = new T[heapCapacity];
			setHeapCapacity(heapCapacity);
		}

		T* dstBuffer = ssoEnabled ? _ssoBuffer : _heap;
		std::memcpy(dstBuffer, str, length * sizeof(T));
		dstBuffer[length] = T();
	}

	template<typename T>
	StaticString<T>::StaticString(const T* str, size_t length) : _size(0), _capacityInfo(0)
	{
		_size = length;

		const bool ssoEnabled = length < StrUtil::kSsoCapacity;
		setHeapAllocated(!ssoEnabled);

		if (ssoEnabled == false)
		{
			const size_t heapCapacity = length + 1;
			_heap = new T[heapCapacity];
			setHeapCapacity(heapCapacity);
		}

		T* dstBuffer = ssoEnabled ? _ssoBuffer : _heap;
		std::memcpy(dstBuffer, str, length * sizeof(T));
		dstBuffer[length] = T();
	}

	template<typename T>
	StaticString<T>::StaticString(const StaticString& other) : _size(0), _capacityInfo(0)
	{
		_size = other._size;
		_capacityInfo = other._capacityInfo;
		const bool srcIsHeap = other.isHeapAllocated();
		const size_t srcLength = other._size;

		if (srcIsHeap)
		{
			const size_t heapCapacity = other.getHeapCapacity();
			_heap = new T[heapCapacity];
			setHeapCapacity(heapCapacity);
		}
		T* dstBuffer = srcIsHeap ? _heap : _ssoBuffer;
		const T* srcBuffer = srcIsHeap ? other._heap : other._ssoBuffer;

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
	StaticString<T>::StaticString(StaticString&& other) noexcept : _size(0), _capacityInfo(0)
	{
		_size = other._size;
		_capacityInfo = other._capacityInfo;
		other._size = 0;
		other._capacityInfo = 0;

		if (isHeapAllocated())
		{
			_heap = other._heap;
			other._heap = nullptr;
		}
		else
		{
			std::memcpy(_ssoBuffer, other._ssoBuffer, StrUtil::kSsoCapacity * sizeof(T));
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
		if (_size != other._size)
			return false;

		const T* lhsBuffer = isHeapAllocated() ? _heap : _ssoBuffer;
		const T* rhsBuffer = other.isHeapAllocated() ? other._heap : other._ssoBuffer;
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
		if (index >= _size)
		{
			KEYH_ASSERT(false, "Index out of bounds");
			return gNullChar;
		}

		T* buffer = isHeapAllocated() ? _heap : _ssoBuffer;
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
		MemoryUtil::swap(_size, other._size);
		MemoryUtil::swap(_capacityInfo, other._capacityInfo);

		char tempBytes[sizeof(_ssoBuffer)];
		std::memcpy(tempBytes, &_ssoBuffer, sizeof(_ssoBuffer));
		std::memcpy(&_ssoBuffer, &other._ssoBuffer, sizeof(_ssoBuffer));
		std::memcpy(&other._ssoBuffer, tempBytes, sizeof(_ssoBuffer));
	}

	template<typename T>
	void StaticString<T>::clear()
	{
		if (isHeapAllocated())
		{
			delete[] _heap;
			_heap = nullptr;
		}
		_size = 0;
		_capacityInfo = 0;
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
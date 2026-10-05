#include "CommonBasePch.h"
#include "DynamicBuffer.h"

namespace keyh
{
	template<typename T, bool isString>
	void DynamicBuffer<T, isString>::allocateInner(size_t size)
	{
		T* newBuffer = new T[size];
		if (_offset != 0)
			memcpy(static_cast<void*>(newBuffer), static_cast<void*>(_buffer), _offset * sizeof(T));
		resetImpl();
		_capacity = size;
		_buffer = newBuffer;
		if constexpr (isString)
			_buffer[_offset] = T();
	}	

	template<typename T, bool isString>
	void DynamicBuffer<T, isString>::allocate(size_t capacity)
	{
		if (capacity <= _capacity)
			return;
		
		allocateInner(capacity);
	}

	template<typename T, bool isString>
	void DynamicBuffer<T, isString>::shrinkToFit()
	{
		const size_t requiredCapacity = _offset + (isString ? 1 : 0);
		if (requiredCapacity == _capacity)
			return;

		allocateInner(requiredCapacity);
	}

	template<typename T, bool isString>
	void DynamicBuffer<T, isString>::resetImpl()
	{
		if (_buffer)
		{
			delete[] _buffer;
			_buffer = nullptr;
		}
		_capacity = 0;
	}

	template class DynamicBuffer<char, false>;
	template class DynamicBuffer<char, true>;
	template class DynamicBuffer<wchar_t, false>;
	template class DynamicBuffer<wchar_t, true>;
	template class DynamicBuffer<byte, false>;
	template class DynamicBuffer<byte, true>;
}

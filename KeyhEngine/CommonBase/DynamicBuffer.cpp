#include "CommonBasePch.h"
#include "DynamicBuffer.h"

namespace keyh
{
	template<typename T>
	void DynamicBuffer<T>::allocate(size_t capacity)
	{
		if (capacity <= _capacity)
			return;
		
		T* newBuffer = new T[capacity];
		memcpy(static_cast<void*>(newBuffer), static_cast<void*>(_buffer), _capacity * sizeof(T));
		_capacity = capacity;

		resetImpl();
		_buffer = newBuffer;
		_buffer[_offset] = T();
	}

	template<typename T>
	void DynamicBuffer<T>::resetImpl()
	{
		if (_buffer)
		{
			delete[] _buffer;
			_buffer = nullptr;
		}
	}

	template class DynamicBuffer<char>;
	template class DynamicBuffer<wchar_t>;

	using DynamicBufferA = DynamicBuffer<char>;
	using DynamicBufferW = DynamicBuffer<wchar_t>;
}
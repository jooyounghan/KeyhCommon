#include "CommonBasePch.h"
#include "DynamicBuffer.h"

namespace keyh
{
	template<typename T>
	void DynamicBuffer<T>::allocateInner(size_t size)
	{
		T* newBuffer = new T[size];
		memcpy(static_cast<void*>(newBuffer), static_cast<void*>(_buffer), _capacity * sizeof(T));
		_capacity = size;

		resetImpl();
		_buffer = newBuffer;
		_buffer[_offset] = T();
	}	

	template<typename T>
	void DynamicBuffer<T>::allocate(size_t capacity)
	{
		if (capacity <= _capacity)
			return;
		
		allocateInner(capacity);
	}

	template<typename T>
	void DynamicBuffer<T>::shrinkToFit()
	{
		if (_offset == _capacity)
			return;

		allocateInner(_offset + 1);
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
}
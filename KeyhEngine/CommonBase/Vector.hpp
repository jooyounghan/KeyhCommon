#include "Vector.h"
namespace keyh
{
	template<typename T>
	Vector<T>::~Vector()
	{
		clear();
	}

	template<typename T>
	Vector<T>::Vector(const Vector& other)
		: _size(other._size)
		, _capacity(other._capacity)
	{
		if (_capacity > 0)
		{
			_data = operator new(_capacity * sizeof(T));
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<T*>(_data) + i) T(static_cast<T*>(other._data)[i]);
			}
		}
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Vector temp(other);
			MemoryUtil::swap(_data, temp._data);
			MemoryUtil::swap(_size, temp._size);
			MemoryUtil::swap(_capacity, temp._capacity);
		}
		return *this;
	}

	template<typename T>
	Vector<T>::Vector(Vector&& other) noexcept
		: _size(other._size)
		, _capacity(other._capacity)
		, _data(other._data)
	{
		other._size = 0;
		other._capacity = 0;
		other._data = nullptr;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector && other) noexcept
	{
		if (this != &other)
		{
			clear();
			_data = other._data;
			_size = other._size;
			_capacity = other._capacity;
			other._data = nullptr;
			other._size = 0;
			other._capacity = 0;
		}
		return *this;
	}

	template<typename T>
	T& Vector<T>::push_back(const T& value)
	{
		if (_size >= _capacity)
		{
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}

		new (static_cast<T*>(_data) + _size) T(value);
		++_size;
		return static_cast<T*>(_data)[_size - 1];
	}

	template<typename T>
	T& Vector<T>::push_back(T&& value)
	{
		return emplace_back(std::move(value));
	}

	template<typename T>
	template<typename ... Args>
	T& Vector<T>::emplace_back(Args&&... args)
	{
		if (_size >= _capacity)
		{
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}
		new (static_cast<T*>(_data) + _size) T(std::forward<Args>(args)...);
		++_size;
		return static_cast<T*>(_data)[_size - 1];
	}

	template<typename T>
	void Vector<T>::pop_back()
	{
		if (_size > 0)
		{
			--_size;
			static_cast<T*>(_data)[_size].~T();
		}
	}

	template<typename T>
	T& Vector<T>::operator[](size_t index)
	{

	}

	template<typename T>
	const T& Vector<T>::operator[](size_t index) const
	{

	}

	template<typename T>
	void Vector<T>::resize(size_t newSize)
	{
	}

	template<typename T>
	void Vector<T>::reserve(size_t newCapacity)
	{
	}

	template<typename T>
	void Vector<T>::shrinkToFit()
	{
	
	}

	template<typename T>
	void Vector<T>::clear()
	{
	}
}
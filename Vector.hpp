#include "Vector.h"
namespace keyh
{
	template<typename T>
	Vector<T>::Iterator::Iterator(T* ptr)
		: _ptr(ptr) {}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++_ptr;
		return temp;
	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const T* ptr)
		: _ptr(ptr) {}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++_ptr;
		return temp;
	}

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
			this->swap(temp);
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
			Vector temp(keyh::move(other));
			this->swap(temp);
		}
		return *this;
	}

#define ADJUST_CAPACITY_IF_NEEDED()	\
	if (_size >= _capacity) { reserve(_capacity == 0 ? 1 : _capacity * MemoryUtil::kCapacityGrowthFactor); }

	template<typename T>
	T& Vector<T>::push_back(const T& value)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		new (static_cast<T*>(_data) + _size) T(value);
		++_size;
		return static_cast<T*>(_data)[_size - 1];
	}

	template<typename T>
	T& Vector<T>::push_back(T&& value)
	{
		return emplace_back(keyh::move(value));
	}

	template<typename T>
	template<typename ... Args>
	T& Vector<T>::emplace_back(Args&&... args)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		new (static_cast<T*>(_data) + _size) T(keyh::forward<Args>(args)...);
		++_size;
		return static_cast<T*>(_data)[_size - 1];
	}

#undef ADJUST_CAPACITY_IF_NEEDED

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
		KEYH_ASSERT(index < _size, "Index out of bounds");
		return static_cast<T*>(_data)[index];
	}

	template<typename T>
	const T& Vector<T>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _size, "Index out of bounds");
		return static_cast<const T*>(_data)[index];
	}

	template<typename T>
	void Vector<T>::erase(size_t index, bool keepOrder)
	{
		KEYH_ASSERT(index < _size, "Index out of bounds");
		if (index >= _size)
			return;

		static_cast<T*>(_data)[index].~T();
		if (keepOrder)
		{
			for (size_t i = index; i < _size - 1; ++i)
			{
				new (static_cast<T*>(_data) + i) T(keyh::move(static_cast<T*>(_data)[i + 1]));
				static_cast<T*>(_data)[i + 1].~T();
			}
		}
		else
		{
			if (index != _size - 1)
			{
				new (static_cast<T*>(_data) + index) T(keyh::move(static_cast<T*>(_data)[_size - 1]));
				static_cast<T*>(_data)[_size - 1].~T();
			}
		}
		_size--;
	}

	template<typename T>
	void Vector<T>::resize(size_t newSize)
	{
		if (newSize > _size)
		{
			if (newSize > _capacity)
			{
				reserve(newSize);
			}
			for (size_t i = _size; i < newSize; ++i)
			{
				new (static_cast<T*>(_data) + i) T();
			}
		}
		else if (newSize < _size)
		{
			for (size_t i = newSize; i < _size; ++i)
			{
				static_cast<T*>(_data)[i].~T();
			}
		}
		_size = newSize;
	}

	template<typename T>
	void Vector<T>::reserve(size_t newCapacity)
	{
		if (newCapacity > _capacity)
		{
			void* newData = operator new(newCapacity * sizeof(T));
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<T*>(newData) + i) T(keyh::move(static_cast<T*>(_data)[i]));
				static_cast<T*>(_data)[i].~T();
			}
			operator delete(_data);
			_data = newData;
			_capacity = newCapacity;
		}
	}

	template<typename T>
	void Vector<T>::shrinkToFit()
	{
		void* newData = operator new(_size * sizeof(T));
		for (size_t i = 0; i < _size; ++i)
		{
			new (static_cast<T*>(newData) + i) T(keyh::move(static_cast<T*>(_data)[i]));
			static_cast<T*>(_data)[i].~T();
		}
		operator delete(_data);
		_data = newData;
		_capacity = _size;
	}

	template<typename T>
	void Vector<T>::clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			static_cast<T*>(_data)[i].~T();
		}
		operator delete(_data);
		_data = nullptr;
		_size = 0;
		_capacity = 0;
	}
	
	template<typename T>
	void Vector<T>::swap(Vector& other) noexcept
	{
		MemoryUtil::swap(_data, other._data);
		MemoryUtil::swap(_size, other._size);
		MemoryUtil::swap(_capacity, other._capacity);
	}
}
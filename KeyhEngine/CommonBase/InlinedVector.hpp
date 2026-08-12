namespace keyh
{
	template<typename T, size_t Count>
	InlinedVector<T, Count>::Iterator::Iterator(T* ptr)
		: _ptr(ptr) {}

	template<typename T, size_t Count>
	typename InlinedVector<T, Count>::Iterator& InlinedVector<T, Count>::Iterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T, size_t Count>
	typename InlinedVector<T, Count>::Iterator InlinedVector<T, Count>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++_ptr;
		return temp;
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>::ConstIterator::ConstIterator(const T* ptr)
		: _ptr(ptr) {}

	template<typename T, size_t Count>
	typename InlinedVector<T, Count>::ConstIterator& InlinedVector<T, Count>::ConstIterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T, size_t Count>
	typename InlinedVector<T, Count>::ConstIterator InlinedVector<T, Count>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++_ptr;
		return temp;
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>::InlinedVector()
	{
		setHeapAllocated(false);
		setHeapCapacity(0);
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>::~InlinedVector()
	{
		clear();
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>::InlinedVector(const InlinedVector& other)
		: InlinedVector()
	{
		reserve(other._size);
		for (size_t i = 0; i < other._size; ++i)
		{
			emplace_back(other[i]);
		}
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>& InlinedVector<T, Count>::operator=(const InlinedVector& other)
	{
		if (this != &other)
		{
			clear();
			reserve(other._size);
			for (size_t i = 0; i < other._size; ++i)
			{
				emplace_back(other[i]);
			}
		}
		return *this;
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>::InlinedVector(InlinedVector&& other) noexcept
		: InlinedVector()
	{
		if (other.isHeapAllocated())
		{
			_storage._heap = other._storage._heap;
			_size = other._size;
			_capacityInfo = other._capacityInfo;

			other._storage._heap = nullptr;
			other._size = 0;
			other._capacityInfo = 0;
			other.setHeapAllocated(false);
		}
		else
		{
			for (size_t i = 0; i < other._size; ++i)
			{
				emplace_back(keyh::move(other[i]));
			}
			other.clear();
		}
	}

	template<typename T, size_t Count>
	InlinedVector<T, Count>& InlinedVector<T, Count>::operator=(InlinedVector&& other) noexcept
	{
		if (this != &other)
		{
			clear();

			if (other.isHeapAllocated())
			{
				_storage._heap = other._storage._heap;
				_size = other._size;
				_capacityInfo = other._capacityInfo;

				other._storage._heap = nullptr;
				other._size = 0;
				other._capacityInfo = 0;
				other.setHeapAllocated(false);
			}
			else
			{
				for (size_t i = 0; i < other._size; ++i)
				{
					emplace_back(keyh::move(other[i]));
				}
				other.clear();
			}
		}
		return *this;
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::destroyRange(size_t beginIndex, size_t endIndex)
	{
		T* buffer = getBuffer();
		for (size_t i = beginIndex; i < endIndex; ++i)
		{
			buffer[i].~T();
		}
	}

	template<typename T, size_t Count>
	T& InlinedVector<T, Count>::push_back(const T& value)
	{
		return emplace_back(value);
	}

	template<typename T, size_t Count>
	T& InlinedVector<T, Count>::push_back(T&& value)
	{
		return emplace_back(keyh::move(value));
	}

	template<typename T, size_t Count>
	template<typename ... Args>
	T& InlinedVector<T, Count>::emplace_back(Args&&... args)
	{
		if (_size >= capacity())
		{
			const size_t newCapacity = capacity() == 0 ? 1 : capacity() * MemoryUtil::kCapacityGrowthFactor;
			reserve(newCapacity);
		}

		T* buffer = getBuffer();
		new (buffer + _size) T(keyh::forward<Args>(args)...);
		++_size;
		return buffer[_size - 1];
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::pop_back()
	{
		if (_size > 0)
		{
			--_size;
			getBuffer()[_size].~T();
		}
	}

	template<typename T, size_t Count>
	T& InlinedVector<T, Count>::operator[](size_t index)
	{
		KEYH_ASSERT(index < _size, "Index out of bounds");
		return getBuffer()[index];
	}

	template<typename T, size_t Count>
	const T& InlinedVector<T, Count>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _size, "Index out of bounds");
		return getBuffer()[index];
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::erase(size_t index, bool keepOrder)
	{
		KEYH_ASSERT(index < _size, "Index out of bounds");
		if (index >= _size)
			return;

		T* buffer = getBuffer();
		buffer[index].~T();
		if (keepOrder)
		{
			for (size_t i = index; i < _size - 1; ++i)
			{
				new (buffer + i) T(keyh::move(buffer[i + 1]));
				buffer[i + 1].~T();
			}
		}
		else if (index != _size - 1)
		{
			new (buffer + index) T(keyh::move(buffer[_size - 1]));
			buffer[_size - 1].~T();
		}

		--_size;
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::resize(size_t newSize)
	{
		if (newSize > _size)
		{
			if (newSize > capacity())
			{
				reserve(newSize);
			}

			T* buffer = getBuffer();
			for (size_t i = _size; i < newSize; ++i)
			{
				new (buffer + i) T();
			}
		}
		else if (newSize < _size)
		{
			destroyRange(newSize, _size);
		}

		_size = newSize;
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::reserve(size_t newCapacity)
	{
		if (newCapacity <= capacity())
			return;

		T* newData = static_cast<T*>(operator new(newCapacity * sizeof(T)));
		T* oldData = getBuffer();
		for (size_t i = 0; i < _size; ++i)
		{
			new (newData + i) T(keyh::move(oldData[i]));
		}

		destroyRange(0, _size);
		if (isHeapAllocated())
		{
			operator delete(_storage._heap);
		}

		_storage._heap = newData;
		setHeapAllocated(true);
		setHeapCapacity(newCapacity);
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::shrinkToFit()
	{
		if (_size <= Count)
		{
			if (isHeapAllocated() == false)
				return;

			T* oldData = _storage._heap;
			for (size_t i = 0; i < _size; ++i)
			{
				new (_storage._ssoBuffer + i) T(keyh::move(oldData[i]));
				oldData[i].~T();
			}

			operator delete(oldData);
			setHeapAllocated(false);
			setHeapCapacity(0);
			return;
		}

		if (isHeapAllocated() == false || _size == getHeapCapacity())
			return;

		T* oldData = _storage._heap;
		T* newData = static_cast<T*>(operator new(_size * sizeof(T)));
		for (size_t i = 0; i < _size; ++i)
		{
			new (newData + i) T(keyh::move(oldData[i]));
			oldData[i].~T();
		}

		operator delete(oldData);
		_storage._heap = newData;
		setHeapCapacity(_size);
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::clear()
	{
		destroyRange(0, _size);
		if (isHeapAllocated())
		{
			operator delete(_storage._heap);
			_storage._heap = nullptr;
		}

		_size = 0;
		_capacityInfo = 0;
		setHeapAllocated(false);
	}

	template<typename T, size_t Count>
	void InlinedVector<T, Count>::swap(InlinedVector& other)
	{
		if (this == &other)
			return;

		if (isHeapAllocated() && other.isHeapAllocated())
		{
			MemoryUtil::swap(_storage._heap, other._storage._heap);
			MemoryUtil::swap(_size, other._size);
			MemoryUtil::swap(_capacityInfo, other._capacityInfo);
			return;
		}

		InlinedVector temp(keyh::move(other));
		for (size_t i = 0; i < _size; ++i)
		{
			other.emplace_back(keyh::move((*this)[i]));
		}

		clear();
		for (size_t i = 0; i < temp._size; ++i)
		{
			emplace_back(keyh::move(temp[i]));
		}
	}
}

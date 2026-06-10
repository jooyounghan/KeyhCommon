namespace keyh
{
	template<typename T>
	Deque<T>::Iterator::Iterator(void* data, size_t top, size_t offset, size_t capacity)
		: _data(data), _top(top), _offset(offset), _capacity(capacity) {}

	template<typename T>
	Deque<T>::Iterator& Deque<T>::Iterator::operator++()
	{
		++_offset;
		return *this;
	}

	template<typename T>
	Deque<T>::Iterator Deque<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++_offset;
		return temp;
	}

	template<typename T>
	Deque<T>::ConstIterator::ConstIterator(const void* data, size_t top, size_t offset, size_t capacity)
		: _data(data), _top(top), _offset(offset), _capacity(capacity) {}

	template<typename T>
	Deque<T>::ConstIterator& Deque<T>::ConstIterator::operator++()
	{
		++_offset;
		return *this;
	}

	template<typename T>
	Deque<T>::ConstIterator Deque<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++_offset;
		return temp;
	}

	template<typename T>
	Deque<T>::~Deque()
	{
		clear();
	}

	template<typename T>
	Deque<T>::Deque(const Deque& other)
		: _top(other._top)
		, _bottom(other._bottom)
		, _capacity(other._capacity)
		, _isEmpty(other._isEmpty)
	{
		if (_capacity > 0)
		{
			_data = operator new(_capacity * sizeof(T));
			for (size_t i = 0; i < size(); ++i)
			{
				const size_t index = getIndex(_top, i, _capacity);
				new (static_cast<T*>(_data) + index) T(static_cast<T*>(other._data)[index]);
			}
		}
	}

	template<typename T>
	Deque<T>& Deque<T>::operator=(const Deque& other)
	{
		if (this != &other)
		{
			Deque temp(other);
			MemoryUtil::swap(_data, temp._data);
			MemoryUtil::swap(_top, temp._top);
			MemoryUtil::swap(_bottom, temp._bottom);
			MemoryUtil::swap(_capacity, temp._capacity);
			MemoryUtil::swap(_isEmpty, temp._isEmpty);
		}
		return *this;
	}

	template<typename T>
	Deque<T>::Deque(Deque&& other) noexcept
		: _top(other._top)
		, _bottom(other._bottom)
		, _capacity(other._capacity)
		, _isEmpty(other._isEmpty)
		, _data(other._data)
	{
		other._top = 0;
		other._bottom = 0;
		other._capacity = 0;
		other._isEmpty = true;
		other._data = nullptr;
	}

	template<typename T>
	Deque<T>& Deque<T>::operator=(Deque&& other) noexcept
	{
		if (this != &other)
		{
			clear();
			_top = other._top;
			_bottom = other._bottom;
			_capacity = other._capacity;
			_isEmpty = other._isEmpty;
			_data = other._data;

			other._top = 0;
			other._bottom = 0;
			other._capacity = 0;
			other._isEmpty = true;
			other._data = nullptr;
		}
		return *this;
	}

#define ADJUST_CAPACITY_IF_NEEDED()	\
	if (size() >= _capacity) { reserve(_capacity == 0 ? 2 : _capacity * MemoryUtil::kCapacityGrowthFactor); }

	template<typename T>
	T& Deque<T>::push_front(const T& value)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		_top = getIndex(_top, -1, _capacity);
		new (static_cast<T*>(_data) + _top) T(value);
		_isEmpty = false;

		return static_cast<T*>(_data)[_top];
	}

	template<typename T>
	T& Deque<T>::push_front(T&& value)
	{
		return emplace_front(move(value));
	}

	template<typename T>
	template<typename... Args>
	T& Deque<T>::emplace_front(Args&&... args)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		_top = getIndex(_top, -1, _capacity);
		new (static_cast<T*>(_data) + _top) T(forward<Args>(args)...);
		_isEmpty = false;
		return static_cast<T*>(_data)[_top];
	}

	template<typename T>
	void Deque<T>::pop_front()
	{
		if (_isEmpty)
			return;

		static_cast<T*>(_data)[_top].~T();
		_top = getIndex(_top, 1, _capacity);
		_isEmpty = (_top == _bottom);
	}

	template<typename T>
	T& Deque<T>::push_back(const T& value)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		size_t insertIndex = _bottom;
		new (static_cast<T*>(_data) + insertIndex) T(value);
		_bottom = getIndex(_bottom, 1, _capacity);
		_isEmpty = false;
		return static_cast<T*>(_data)[insertIndex];
	}

	template<typename T>
	T& Deque<T>::push_back(T&& value)
	{
		return emplace_back(move(value));
	}

	template<typename T>
	template<typename... Args>
	T& Deque<T>::emplace_back(Args&&... args)
	{
		ADJUST_CAPACITY_IF_NEEDED();
		size_t insertIndex = _bottom;
		new (static_cast<T*>(_data) + insertIndex) T(forward<Args>(args)...);
		_bottom = getIndex(_bottom, 1, _capacity);
		_isEmpty = false;
		return static_cast<T*>(_data)[insertIndex];
	}

#undef ADJUST_CAPACITY_IF_NEEDED

	template<typename T>
	void Deque<T>::pop_back()
	{
		if (_isEmpty)
			return;
		_bottom = getIndex(_bottom, -1, _capacity);
		static_cast<T*>(_data)[_bottom].~T();
		_isEmpty = (_top == _bottom);
	}

	template<typename T>
	T& Deque<T>::front() 
	{ 
		KEYH_ASSERT(size() > 0, "Deque is empty");
		return static_cast<T*>(_data)[_top]; 
	}

	template<typename T>
	const T& Deque<T>::front() const 
	{ 
		KEYH_ASSERT(size() > 0, "Deque is empty");
		return static_cast<const T*>(_data)[_top]; 
	}

	template<typename T>
	T& Deque<T>::back() 
	{ 
		KEYH_ASSERT(size() > 0, "Deque is empty");
		return static_cast<T*>(_data)[getIndex(_bottom, -1, _capacity)]; 
	}

	template<typename T>
	const T& Deque<T>::back() const 
	{ 
		KEYH_ASSERT(size() > 0, "Deque is empty");
		return static_cast<const T*>(_data)[getIndex(_bottom, -1, _capacity)]; 
	}

	template<typename T>
	T& Deque<T>::operator[](size_t index)
	{
		KEYH_ASSERT(index < size(), "Index out of bounds");
		return static_cast<T*>(_data)[getIndex(_top, index, _capacity)];
	}

	template<typename T>
	const T& Deque<T>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < size(), "Index out of bounds");
		return static_cast<const T*>(_data)[getIndex(_top, index, _capacity)];
	}

	template<typename T>
	void Deque<T>::erase(size_t index, bool keepOrder)
	{
		const size_t count = size();
		KEYH_ASSERT(index < count, "Index out of bounds");
		if (index >= count)
			return;

		static_cast<T*>(_data)[getIndex(_top, index, _capacity)].~T();

		if (keepOrder)
		{
			const size_t halfSize = count / 2;
			if (index < halfSize)
			{
				for (size_t i = index; i > 0; --i)
				{
					size_t fromIndex = getIndex(_top, i - 1, _capacity);
					size_t toIndex = getIndex(_top, i, _capacity);
					new (static_cast<T*>(_data) + toIndex) T(move(static_cast<T*>(_data)[fromIndex]));
					static_cast<T*>(_data)[fromIndex].~T();
				}
				_top = getIndex(_top, 1, _capacity);
			}
			else
			{
				for (size_t i = index; i < count - 1; ++i)
				{
					size_t fromIndex = getIndex(_top, i + 1, _capacity);
					size_t toIndex = getIndex(_top, i, _capacity);
					new (static_cast<T*>(_data) + toIndex) T(move(static_cast<T*>(_data)[fromIndex]));
					static_cast<T*>(_data)[fromIndex].~T();
				}
				_bottom = getIndex(_bottom, -1, _capacity);
			}
		}
		else
		{
			if (index != count - 1)
			{
				size_t targetIndex = getIndex(_top, index, _capacity);
				size_t lastIndex = getIndex(_top, count - 1, _capacity);
				new (static_cast<T*>(_data) + targetIndex) T(move(static_cast<T*>(_data)[lastIndex]));
			}
			pop_back();
		}
	}

	template<typename T>
	void Deque<T>::resize(size_t newSize)
	{
		const size_t count = size();
		if (newSize > count)
		{
			if (newSize > _capacity)
			{
				reserve(newSize);
			}
			for (size_t i = count; i < newSize; ++i)
			{
				new (static_cast<T*>(_data) + getIndex(_top, i, _capacity)) T();
			}
		}
		else if (newSize < count)
		{
			for (size_t i = newSize; i < count; ++i)
			{
				static_cast<T*>(_data)[getIndex(_top, i, _capacity)].~T();
			}
		}
	}

	template<typename T>
	void Deque<T>::reserve(size_t newCapacity)
	{
		if (newCapacity <= _capacity)
			return;

		void* newData = operator new(newCapacity * sizeof(T));
		const size_t count = size();
		for (size_t i = 0; i < count; ++i)
		{
			const size_t index = getIndex(_top, i, _capacity);
			new (static_cast<T*>(newData) + i) T(move(static_cast<T*>(_data)[index]));
			static_cast<T*>(_data)[index].~T();
		}
		operator delete(_data);
		_data = newData;
		_capacity = newCapacity;
		_top = 0;
		_bottom = count;
		_isEmpty = (count == 0);
	}

	template<typename T>
	void Deque<T>::clear()
	{
		const size_t count = size();
		for (size_t i = 0; i < count; ++i)
		{
			const size_t index = getIndex(_top, i, _capacity);
			static_cast<T*>(_data)[index].~T();
		}

		operator delete(_data);
		_data = nullptr;
		_capacity = 0;
		_top = 0;
		_bottom = 0;
		_isEmpty = true;
	}
}
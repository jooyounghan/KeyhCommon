namespace keyh
{
	template<typename T>
	void Queue<T>::push(const T& value)
	{
		_deque.push_back(value);
	}

	template<typename T>
	void Queue<T>::push(T&& value)
	{
		_deque.push_back(keyh::move(value));
	}

	template<typename T>
	void Queue<T>::pop()
	{
		_deque.pop_front();
	}

	template<typename T>
	void Queue<T>::resize(size_t newSize)
	{
		_deque.resize(newSize);
	}

	template<typename T>
	void Queue<T>::reserve(size_t newCapacity)
	{
		_deque.reserve(newCapacity);
	}

	template<typename T>
	void Queue<T>::clear()
	{
		_deque.clear();
	}

	template<typename T>
	void Queue<T>::swap(Queue& other) noexcept
	{
		_deque.swap(other._deque);
	}
}

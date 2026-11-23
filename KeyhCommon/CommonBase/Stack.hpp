namespace keyh
{
	template<typename T>
	void Stack<T>::push(const T& value)
	{
		_deque.push_back(value);
	}

	template<typename T>
	void Stack<T>::push(T&& value)
	{
		_deque.push_back(keyh::move(value));
	}

	template<typename T>
	void Stack<T>::pop()
	{
		_deque.pop_back();
	}

	template<typename T>
	void Stack<T>::resize(size_t newSize)
	{
		_deque.resize(newSize);
	}

	template<typename T>
	void Stack<T>::reserve(size_t newCapacity)
	{
		_deque.reserve(newCapacity);
	}

	template<typename T>
	void Stack<T>::clear()
	{
		_deque.clear();
	}

	template<typename T>
	void Stack<T>::swap(Stack& other) noexcept
	{
		_deque.swap(other._deque);
	}
}

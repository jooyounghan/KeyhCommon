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
	void Queue<T>::clear()
	{
		_deque.clear();
	}
}

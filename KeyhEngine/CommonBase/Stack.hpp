#include <stack>
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
	void Stack<T>::clear()
	{
		_deque.clear();
	}
}
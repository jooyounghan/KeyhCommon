#pragma once
#include "Deque.h"

namespace keyh
{
	template<typename T>
	class Stack
	{
	private:
		Deque<T> _deque;

	public:
		void push(const T& value);
		void push(T&& value);
		void pop();
		void resize(size_t newSize);
		void reserve(size_t newCapacity);
		void clear();
		void swap(Stack& other) noexcept;

	public:
		inline T& top() { return _deque.back(); }
		inline const T& top() const { return _deque.back(); }
		inline bool empty() const { return _deque.empty(); }
		inline size_t size() const { return _deque.size(); }
	};
}
#include "Stack.hpp"

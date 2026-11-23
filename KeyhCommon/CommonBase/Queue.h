#pragma once
#include "Deque.h"

namespace keyh
{
	template<typename T>
	class Queue
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
		void swap(Queue& other) noexcept;

	public:
		inline T& front() { return _deque.front(); }
		inline const T& front() const { return _deque.front(); }
		inline T& back() { return _deque.back(); }
		inline const T& back() const { return _deque.back(); }
		inline bool empty() const { return _deque.empty(); }
		inline size_t size() const { return _deque.size(); }
	};
}
#include "Queue.hpp"

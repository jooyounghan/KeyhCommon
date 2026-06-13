#pragma once
#include "MemoryUtil.h"
#include "AssertUtil.h"
#include "CircularBufferUtil.h"

namespace keyh
{
	template<typename T>
	class Deque
	{
	public:
		class Iterator
		{
		public:
			Iterator(void* data, size_t top, size_t offset, size_t capacity);

		public:
			inline T& operator*()				{ return static_cast<T*>(_data)[CircularBufferUtil::getIndex(_top, _offset, _capacity)]; }
			inline const T& operator*() const	{ return static_cast<const T*>(_data)[CircularBufferUtil::getIndex(_top, _offset, _capacity)]; }
			inline bool operator==(const Iterator& other) const { return _offset == other._offset && _data == other._data; }
			inline bool operator!=(const Iterator& other) const { return !(*this == other); }

		public:
			Iterator& operator++();
			Iterator operator++(int);

		private:
			void* _data;
			size_t _top;
			size_t _offset;
			size_t _capacity;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const void* data, size_t top, size_t offset, size_t capacity);

		public:
			inline const T& operator*() const { return static_cast<const T*>(_data)[CircularBufferUtil::getIndex(_top, _offset, _capacity)]; }
			inline bool operator==(const ConstIterator& other) const { return _offset == other._offset && _data == other._data; }
			inline bool operator!=(const ConstIterator& other) const { return !(*this == other); }

		public:
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const void* _data;
			size_t _top;
			size_t _offset;
			size_t _capacity;
		};

	public:
		Deque() = default;
		~Deque();

	public:
		Deque(const Deque& other);
		Deque& operator=(const Deque& other);

	public:
		Deque(Deque&& other) noexcept;
		Deque& operator=(Deque&& other) noexcept;

	private:
		size_t	_top = 0;
		size_t	_bottom = 0;
		size_t	_capacity = 0;
		bool	_isEmpty = true;
		void* _data = nullptr;

	public:
		T& push_front(const T& value);
		T& push_front(T&& value);
		template<typename... Args>
		T& emplace_front(Args&&... args);
		void pop_front();

	public:
		T& push_back(const T& value);
		T& push_back(T&& value);
		template<typename... Args>
		T& emplace_back(Args&&... args);
		void pop_back();

	public:
		inline Iterator begin() { return Iterator(_data, _top, 0, _capacity); }
		inline Iterator end() { return Iterator(_data, _top, size(), _capacity); }
		inline ConstIterator begin() const { return ConstIterator(_data, _top, 0, _capacity); }
		inline ConstIterator end() const { return ConstIterator(_data, _top, size(), _capacity); }

	public:
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;

	public:
		T& operator[](size_t index);
		const T& operator[](size_t index) const;

	public:
		void erase(size_t index, bool keepOrder = true);

	public:
		void resize(size_t newSize);
		void reserve(size_t newCapacity);
		void clear();

	public:
		inline size_t size() const { return _isEmpty ? 0 : (_top >= _bottom ? _bottom + _capacity - _top : _bottom - _top); }
		inline size_t capacity() const { return _capacity; }
	};
}

#include "Deque.hpp"
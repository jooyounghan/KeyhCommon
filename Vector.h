#pragma once
#include "MemoryUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	template<typename T>
	class Vector
	{
	public:
		class Iterator
		{
		public:
			Iterator(T* ptr);

		public:
			inline T& operator*() { return *_ptr; }
			inline const T& operator*() const { return *_ptr; }
			inline T* operator->() { return _ptr; }
			inline const T* operator->() const { return _ptr; }
			inline operator T* () { return _ptr; }
			inline operator const T* () const { return _ptr; }
			inline bool operator==(const Iterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const Iterator& other) const { return !(*this == other); }
			inline Iterator operator+(ptrdiff_t offset) const { return Iterator(_ptr + offset); }
			inline Iterator operator-(ptrdiff_t offset) const { return Iterator(_ptr - offset); }
			inline ptrdiff_t operator-(const Iterator& other) const { return _ptr - other._ptr; }

		public:
			Iterator& operator++();
			Iterator operator++(int);

		private:
			T* _ptr;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const T* ptr);

		public:
			inline const T& operator*() const { return *_ptr; }
			inline const T* operator->() const { return _ptr; }
			inline operator const T* () const { return _ptr; }
			inline bool operator==(const ConstIterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const ConstIterator& other) const { return !(*this == other); }
			inline ConstIterator operator+(ptrdiff_t offset) const { return ConstIterator(_ptr + offset); }
			inline ConstIterator operator-(ptrdiff_t offset) const { return ConstIterator(_ptr - offset); }
			inline ptrdiff_t operator-(const ConstIterator& other) const { return _ptr - other._ptr; }

		public:
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const T* _ptr;
		};

	public:
		Vector() = default;
		~Vector();

	public:
		Vector(const Vector& other);
		Vector& operator=(const Vector& other);

	public:
		Vector(Vector&& other) noexcept;
		Vector& operator=(Vector&& other) noexcept;
	
	private:
		size_t	_size = 0;
		size_t	_capacity = 0;
		void*	_data = nullptr;

	public:
		T& push_back(const T& value);
		T& push_back(T&& value);
		template<typename... Args>
		T& emplace_back(Args&&... args);
		void pop_back();

	public:
		inline Iterator begin() { return Iterator(static_cast<T*>(_data)); }
		inline Iterator end() { return Iterator(static_cast<T*>(_data) + _size); }
		inline ConstIterator begin() const { return ConstIterator(static_cast<const T*>(_data)); }
		inline ConstIterator end() const { return ConstIterator(static_cast<const T*>(_data) + _size); }

	public:
		T& operator[](size_t index);
		const T& operator[](size_t index) const;

	public:
		void erase(size_t index, bool keepOrder = true);

	public:
		void resize(size_t newSize);
		void reserve(size_t newCapacity);
		void shrinkToFit();
		void clear();
		void swap(Vector& other) noexcept;

	public:
		inline T*			data() { return static_cast<T*>(_data); }
		inline const T*		data() const { return static_cast<const T*>(_data); }
		inline size_t		size() const { return _size; }
		inline size_t		capacity() const { return _capacity; }
		inline bool			empty() const { return _size == 0; }

	public:
		inline T&			unsafeFront() { return (*this)[0]; }
		inline const T&		unsafeFront() const { return (*this)[0]; }
		inline T&			unsafeBack() { return (*this)[_size - 1]; }
		inline const T&		unsafeBack() const { return (*this)[_size - 1]; }
	};
}
#include "Vector.hpp"

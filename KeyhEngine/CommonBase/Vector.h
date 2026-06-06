#pragma once
#include "MemoryUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	template<typename T>
	class Vector
	{
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
		inline T* begin() { return static_cast<T*>(_data); }
		inline T* end() { return static_cast<T*>(_data) + _size; }
		inline const T* begin() const { return static_cast<const T*>(_data); }
		inline const T* end() const { return static_cast<const T*>(_data) + _size; }

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

	public:
		inline size_t size() const { return _size; }
		inline size_t capacity() const { return _capacity; }
	};
}
#include "Vector.hpp"

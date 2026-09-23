#pragma once
#include "CommonCore.h"
namespace keyh
{
	template<typename T, size_t Size>
	class StaticArray
	{
	public:
		class Iterator
		{
		public:
			constexpr Iterator(T* ptr) : _ptr(ptr) {}

		public:
			inline constexpr T& operator*() { return *_ptr; }
			inline constexpr const T& operator*() const { return *_ptr; }
			inline constexpr T* operator->() { return _ptr; }
			inline constexpr const T* operator->() const { return _ptr; }
			inline constexpr operator T* () { return _ptr; }
			inline constexpr operator const T* () const { return _ptr; }
			inline constexpr bool operator==(const Iterator& other) const { return _ptr == other._ptr; }
			inline constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }
			inline constexpr Iterator operator+(ptrdiff_t offset) const { return Iterator(_ptr + offset); }
			inline constexpr Iterator operator-(ptrdiff_t offset) const { return Iterator(_ptr - offset); }
			inline constexpr ptrdiff_t operator-(const Iterator& other) const { return _ptr - other._ptr; }

		public:
			inline constexpr Iterator& operator++()
			{
				++_ptr;
				return *this;
			}

			inline constexpr Iterator operator++(int)
			{
				Iterator temp = *this;
				++_ptr;
				return temp;
			}

		private:
			T* _ptr;
		};

		class ConstIterator
		{
		public:
			constexpr ConstIterator(const T* ptr) : _ptr(ptr) {}

		public:
			inline constexpr const T& operator*() const { return *_ptr; }
			inline constexpr const T* operator->() const { return _ptr; }
			inline constexpr operator const T* () const { return _ptr; }
			inline constexpr bool operator==(const ConstIterator& other) const { return _ptr == other._ptr; }
			inline constexpr bool operator!=(const ConstIterator& other) const { return !(*this == other); }
			inline constexpr ConstIterator operator+(ptrdiff_t offset) const { return ConstIterator(_ptr + offset); }
			inline constexpr ConstIterator operator-(ptrdiff_t offset) const { return ConstIterator(_ptr - offset); }
			inline constexpr ptrdiff_t operator-(const ConstIterator& other) const { return _ptr - other._ptr; }

		public:
			inline constexpr ConstIterator& operator++()
			{
				++_ptr;
				return *this;
			}

			inline constexpr ConstIterator operator++(int)
			{
				ConstIterator temp = *this;
				++_ptr;
				return temp;
			}

		private:
			const T* _ptr;
		};

	public:
		constexpr StaticArray() : _data{} {}
		constexpr ~StaticArray() = default;

	public:
		constexpr StaticArray(const StaticArray& other) = default;
		constexpr StaticArray& operator=(const StaticArray& other) = default;
		constexpr StaticArray(StaticArray&& other) noexcept = default;
		constexpr StaticArray& operator=(StaticArray&& other) noexcept = default;

	private:
		T _data[Size];

	public:
		inline constexpr Iterator begin() { return Iterator(_data); }
		inline constexpr ConstIterator begin() const { return ConstIterator(_data); }
		inline constexpr Iterator end() { return Iterator(_data + Size); }
		inline constexpr ConstIterator end() const { return ConstIterator(_data + Size); }

	public:
		constexpr T& operator[](size_t index) { return _data[index]; }
		constexpr const T& operator[](size_t index) const { return _data[index]; }

	public:
		constexpr const T* data() const noexcept { return _data; }
		constexpr size_t size() noexcept { return Size; }
	};

}
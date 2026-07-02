#pragma once
#include "CommonCore.h"
namespace keyh
{
	template<typename T, size_t Size>
	class StaticArray
	{
	public:
		constexpr StaticArray() = default;
		constexpr ~StaticArray() = default;

	public:
		constexpr StaticArray(const StaticArray& other) = default;
		constexpr StaticArray& operator=(const StaticArray& other) = default;
		constexpr StaticArray(StaticArray&& other) noexcept = default;
		constexpr StaticArray& operator=(StaticArray&& other) noexcept = default;

	private:
		T _data[Size];

	public:
		inline constexpr T* begin() { return _data; }
		inline constexpr const T* begin() const { return _data; }
		inline constexpr T* end() { return _data + Size; }
		inline constexpr const T* end() const { return _data + Size; }

	public:
		constexpr T& operator[](size_t index) { return _data[index]; }
		constexpr const T& operator[](size_t index) const { return _data[index]; }

	public:
		constexpr const T* data() const noexcept { return _data; }
		constexpr size_t size() noexcept { return Size; }
	};

}
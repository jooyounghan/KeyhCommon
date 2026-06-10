#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct MemoryUtil
	{
		constexpr static size_t kCapacityGrowthFactor = 2;
		static_assert(kCapacityGrowthFactor > 1, "Capacity growth factor must be greater than 1 to ensure growth.");
		static_assert((kCapacityGrowthFactor& (kCapacityGrowthFactor - 1)) == 0, "Capacity growth factor must be a power of 2 for optimal performance.");

		constexpr static size_t kPageThresholdSize = 4096;
		constexpr static size_t kCachelineAlignSize = (sizeof(void*) == 8) ? 64 : 32;

		template<typename T>
		static constexpr T* addressOf(T& arg) noexcept;

		template<typename T>
		static constexpr T align(T input, TypeIdentity<T> alignedValue) noexcept;

		template<typename T>
		static constexpr bool hasSingleBit(T value) noexcept;

		template<typename T>
		static constexpr T bitCeil(T value) noexcept;

		template <typename T1, typename... Types>
		static constexpr size_t alignOf() noexcept;

		template<size_t ColumnIndex, typename... Types>
		static constexpr size_t getOffset() noexcept;

	private:
		template <typename T>
		static constexpr size_t getPaddedSizeOfImpl() noexcept;
		template <typename T1, typename T2, typename... Types>
		static constexpr size_t getPaddedSizeOfImpl() noexcept;

	public:
		template <typename ...Types>
		static constexpr size_t getPaddedSizeOf() noexcept;

		template <typename T, typename... Types>
		static constexpr size_t getSizeOf() noexcept;

	public:
		template <bool InitializeNull, typename ...Types>
		static void* alignedMalloc(size_t count);

		template<typename ...Types>
		static void alignedFree(void* aligned);

		template<typename T>
		static void safeRelease(T*& ptr) noexcept;

	public:
		template <typename T>
		static void swap(T& a, T& b) noexcept;
	};
}
#include "MemoryUtil.hpp"
#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct MathUtil
	{
        template<typename T, typename Pred>
        static constexpr T max(T left, TypeIdentity<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T max(T left, TypeIdentity<T> right) noexcept;

        template<typename T, typename Pred>
        static constexpr T min(T left, TypeIdentity<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T min(T left, TypeIdentity<T> right) noexcept;

        template<typename T>
        static constexpr T clamp(T input, TypeIdentity<T> min, TypeIdentity<T> max) noexcept;

        template<typename T>
        static constexpr T saturate(T input) noexcept;

        static constexpr bool isPrime(size_t n);

        static constexpr size_t nextPrime(size_t n);
	};
}
#include "MathUtil.hpp"
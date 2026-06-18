#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct MathUtil
	{
#pragma region Operations
        template<typename T, typename Pred>
        static constexpr T max(T left, TypeIdentity_t<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T max(T left, TypeIdentity_t<T> right) noexcept;

        template<typename T, typename Pred>
        static constexpr T min(T left, TypeIdentity_t<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T min(T left, TypeIdentity_t<T> right) noexcept;

        template<typename T>
        static constexpr T clamp(T input, TypeIdentity_t<T> min, TypeIdentity_t<T> max) noexcept;

        template<typename T>
        static constexpr T saturate(T input) noexcept;
#pragma endregion

#pragma region Prime Number
        template<typename T>
        static constexpr bool isPrime(T n);

        template<typename T>
        static constexpr T nextPrime(T n);
#pragma endregion
	};
}
#include "MathUtil.hpp"
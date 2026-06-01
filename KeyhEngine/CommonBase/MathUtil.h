#pragma once

namespace keyh
{
	struct MathUtil
	{
        template<typename T, typename Pred>
        static constexpr T max(T left, std::type_identity_t<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T max(T left, std::type_identity_t<T> right) noexcept;

        template<typename T, typename Pred>
        static constexpr T min(T left, std::type_identity_t<T> right, Pred pred) noexcept;

        template<typename T>
        static constexpr T min(T left, std::type_identity_t<T> right) noexcept;

        template<typename T>
        static constexpr T clamp(T input, std::type_identity_t<T> min, std::type_identity_t<T> max) noexcept;

        template<typename T>
        static constexpr T saturate(T input) noexcept;

        static constexpr bool isPrime(size_t n);

        static constexpr size_t nextPrime(size_t n);
	};
}
#include "MathUtil.hpp"
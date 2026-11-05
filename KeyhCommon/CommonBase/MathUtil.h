#pragma once
#include "CommonCore.h"

namespace keyh
{
#define KEYH_PI 3.14159265358979323846

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

        template<typename T>
        static constexpr bool isEqual(T left, TypeIdentity_t<T> right, T epsilon = static_cast<T>(1.0e-6)) noexcept;

        template<typename T>
        static constexpr bool isLess(T left, TypeIdentity_t<T> right, T epsilon = static_cast<T>(1.0e-6)) noexcept;

        template<typename T>
        static constexpr bool isLessEqual(T left, TypeIdentity_t<T> right, T epsilon = static_cast<T>(1.0e-6)) noexcept;

        template<typename T>
        static constexpr bool isGreater(T left, TypeIdentity_t<T> right, T epsilon = static_cast<T>(1.0e-6)) noexcept;

        template<typename T>
        static constexpr bool isGreaterEqual(T left, TypeIdentity_t<T> right, T epsilon = static_cast<T>(1.0e-6)) noexcept;
#pragma endregion

#pragma region Prime Number
        template<typename T>
        static constexpr bool isPrime(T n);

        template<typename T>
        static constexpr T nextPrime(T n);
#pragma endregion

#pragma region Angle
		template <typename T>
		static constexpr T degreesToRadians(T degrees) noexcept;
        
		template <typename T>
		static constexpr T radiansToDegrees(T radians) noexcept;
#pragma endregion
	};
}
#include "MathUtil.hpp"

namespace keyh
{
    template<typename T, typename Pred>
    constexpr T MathUtil::max(T left, std::type_identity_t<T> right, Pred pred) noexcept
    {
        return pred(left, right) ? right : left;
    }

    template<typename T>
    constexpr T MathUtil::max(T left, std::type_identity_t<T> right) noexcept
    {
        return left < right ? right : left;
    }

    template<typename T, typename Pred>
    constexpr T MathUtil::min(T left, std::type_identity_t<T> right, Pred pred) noexcept
    {
        return pred(left, right) ? left : right;
    }

    template<typename T>
    constexpr T MathUtil::min(T left, std::type_identity_t<T> right) noexcept
    {
        return left < right ? left : right;
    }

    template<typename T>
    constexpr T MathUtil::clamp(T input, std::type_identity_t<T> minLimit, std::type_identity_t<T> maxLimit) noexcept
    {
        return MathUtil::max(MathUtil::min(input, maxLimit), minLimit);
    }

    template<typename T>
    constexpr T MathUtil::saturate(T input) noexcept
    {
        return MathUtil::clamp(input, static_cast<T>(0), static_cast<T>(1));
    }

    constexpr bool MathUtil::isPrime(size_t n)
    {
        if (n < 2) return false;
        for (size_t i = 2; i * i <= n; ++i)
        {
            if (n % i == 0) return false;
        }
        return true;
    }

    constexpr size_t MathUtil::nextPrime(size_t n)
    {
        while (!isPrime(++n)) {}
        return n;
    }
}
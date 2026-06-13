namespace keyh
{
    template<typename T, typename Pred>
    constexpr T MathUtil::max(T left, TypeIdentity<T> right, Pred pred) noexcept
    {
        return pred(left, right) ? right : left;
    }

    template<typename T>
    constexpr T MathUtil::max(T left, TypeIdentity<T> right) noexcept
    {
        return left < right ? right : left;
    }

    template<typename T, typename Pred>
    constexpr T MathUtil::min(T left, TypeIdentity<T> right, Pred pred) noexcept
    {
        return pred(left, right) ? left : right;
    }

    template<typename T>
    constexpr T MathUtil::min(T left, TypeIdentity<T> right) noexcept
    {
        return left < right ? left : right;
    }

    template<typename T>
    constexpr T MathUtil::clamp(T input, TypeIdentity<T> minLimit, TypeIdentity<T> maxLimit) noexcept
    {
        return MathUtil::max(MathUtil::min(input, maxLimit), minLimit);
    }

    template<typename T>
    constexpr T MathUtil::saturate(T input) noexcept
    {
        return MathUtil::clamp(input, static_cast<T>(0), static_cast<T>(1));
    }

    template<typename T>
    constexpr bool MathUtil::isPrime(T n)
    {
        if (n < 2) return false;
        for (T i = 2; i * i <= n; ++i)
        {
            if (n % i == 0) return false;
        }
        return true;
    }

    template<typename T>
    constexpr T MathUtil::nextPrime(T n)
    {
        T limit = (n == 0) ? 2 : n * 2;
        for (T candidate = n + 1; candidate <= limit; ++candidate)
        {
            if (isPrime(candidate))
            {
                return candidate;
            }
        }
        return n + 1;
    }
}
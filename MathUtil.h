#pragma once

#include <cassert>
#include <cstddef>

namespace keyh
{
class MathUtil
{
public:
    static constexpr bool IsPowerOfTwo(std::size_t value) noexcept
    {
        return value != 0 && (value & (value - 1)) == 0;
    }

    static constexpr std::size_t NextPowerOfTwo(std::size_t value) noexcept
    {
        if (value <= 1)
        {
            return 1;
        }

        --value;
        for (std::size_t i = 1; i < sizeof(std::size_t) * 8; i <<= 1)
        {
            value |= value >> i;
        }

        return value + 1;
    }

    template <typename T>
    static constexpr T Max(const T& lhs, const T& rhs) noexcept
    {
        return lhs < rhs ? rhs : lhs;
    }

    template <typename T>
    static constexpr T Min(const T& lhs, const T& rhs) noexcept
    {
        return rhs < lhs ? rhs : lhs;
    }

    static constexpr std::size_t AlignUp(std::size_t value, std::size_t alignment) noexcept
    {
        assert(alignment != 0 && IsPowerOfTwo(alignment));
        return (value + alignment - 1) & ~(alignment - 1);
    }
};
} // namespace keyh

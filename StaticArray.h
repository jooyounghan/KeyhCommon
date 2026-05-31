#pragma once

#include <array>
#include <cstddef>

namespace keyh
{
template <typename T, std::size_t N>
class StaticArray
{
public:
    using Storage = std::array<T, N>;

    constexpr T& operator[](std::size_t index) noexcept { return storage_[index]; }
    constexpr const T& operator[](std::size_t index) const noexcept { return storage_[index]; }

    [[nodiscard]] constexpr std::size_t Size() const noexcept { return N; }

    constexpr T* Data() noexcept { return storage_.data(); }
    constexpr const T* Data() const noexcept { return storage_.data(); }

    constexpr auto begin() noexcept { return storage_.begin(); }
    constexpr auto end() noexcept { return storage_.end(); }
    constexpr auto begin() const noexcept { return storage_.begin(); }
    constexpr auto end() const noexcept { return storage_.end(); }

private:
    Storage storage_{};
};
} // namespace keyh

#pragma once

#include "MemoryUtil.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace keyh
{
template <typename T, std::size_t N>
class StaticVector
{
public:
    StaticVector() = default;

    [[nodiscard]] constexpr std::size_t Size() const noexcept { return size_; }
    [[nodiscard]] constexpr std::size_t Capacity() const noexcept { return N; }
    [[nodiscard]] constexpr bool Empty() const noexcept { return size_ == 0; }

    T* begin() noexcept { return PtrAt(0); }
    T* end() noexcept { return PtrAt(size_); }
    const T* begin() const noexcept { return PtrAt(0); }
    const T* end() const noexcept { return PtrAt(size_); }
    const T* cbegin() const noexcept { return PtrAt(0); }
    const T* cend() const noexcept { return PtrAt(size_); }

    T& operator[](std::size_t index) noexcept
    {
        assert(index < size_);
        return *PtrAt(index);
    }

    const T& operator[](std::size_t index) const noexcept
    {
        assert(index < size_);
        return *PtrAt(index);
    }

    template <typename... Args>
    T& EmplaceBack(Args&&... args)
    {
        assert(size_ < N);
        T* ptr = PtrAt(size_);
        MemoryUtil::ConstructAt(ptr, std::forward<Args>(args)...);
        ++size_;
        return *ptr;
    }

    void PushBack(const T& value) { EmplaceBack(value); }
    void PushBack(T&& value) { EmplaceBack(std::move(value)); }

    void PopBack()
    {
        assert(size_ > 0);
        --size_;
        MemoryUtil::DestroyAt(PtrAt(size_));
    }

    void Clear() noexcept
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            MemoryUtil::DestroyAt(PtrAt(i));
        }
        size_ = 0;
    }

    ~StaticVector()
    {
        Clear();
    }

private:
    T* PtrAt(std::size_t index) noexcept
    {
        return reinterpret_cast<T*>(raw_ + index * sizeof(T));
    }

    const T* PtrAt(std::size_t index) const noexcept
    {
        return reinterpret_cast<const T*>(raw_ + index * sizeof(T));
    }

    alignas(T) std::byte raw_[sizeof(T) * N]{};
    std::size_t size_ = 0;
};
} // namespace keyh

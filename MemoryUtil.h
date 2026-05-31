#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

namespace keyh
{
class MemoryUtil
{
public:
    template <typename T>
    static T* Allocate(std::size_t count)
    {
        return static_cast<T*>(::operator new(sizeof(T) * count));
    }

    template <typename T>
    static void Deallocate(T* ptr) noexcept
    {
        ::operator delete(ptr);
    }

    template <typename T, typename... Args>
    static T* ConstructAt(T* ptr, Args&&... args)
    {
        return std::construct_at(ptr, std::forward<Args>(args)...);
    }

    template <typename T>
    static void DestroyAt(T* ptr) noexcept
    {
        std::destroy_at(ptr);
    }

    template <typename T>
    static void DestroyRange(T* ptr, std::size_t count) noexcept
    {
        std::destroy_n(ptr, count);
    }

    template <typename T>
    static T* UninitializedMove(T* first, T* last, T* dest)
    {
        return std::uninitialized_move(first, last, dest);
    }

    template <typename T>
    static T* UninitializedCopy(const T* first, const T* last, T* dest)
    {
        return std::uninitialized_copy(first, last, dest);
    }
};
} // namespace keyh

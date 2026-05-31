#pragma once

#include "MathUtil.h"
#include "MemoryUtil.h"

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace keyh
{
template <typename T>
class Vector
{
public:
    Vector() = default;

    explicit Vector(std::size_t count)
    {
        Resize(count);
    }

    Vector(std::initializer_list<T> init)
    {
        Reserve(init.size());
        for (const T& value : init)
        {
            EmplaceBack(value);
        }
    }

    Vector(const Vector& other)
    {
        Reserve(other.size_);
        MemoryUtil::UninitializedCopy(other.data_, other.data_ + other.size_, data_);
        size_ = other.size_;
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector& operator=(const Vector& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Clear();
        Reserve(other.size_);
        MemoryUtil::UninitializedCopy(other.data_, other.data_ + other.size_, data_);
        size_ = other.size_;
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ReleaseStorage();
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    ~Vector()
    {
        ReleaseStorage();
    }

    [[nodiscard]] std::size_t Size() const noexcept { return size_; }
    [[nodiscard]] std::size_t Capacity() const noexcept { return capacity_; }
    [[nodiscard]] bool Empty() const noexcept { return size_ == 0; }

    T* Data() noexcept { return data_; }
    const T* Data() const noexcept { return data_; }

    T* begin() noexcept { return data_; }
    T* end() noexcept { return data_ + size_; }
    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept { return data_ + size_; }
    const T* cbegin() const noexcept { return data_; }
    const T* cend() const noexcept { return data_ + size_; }

    T& operator[](std::size_t index) noexcept
    {
        assert(index < size_);
        return data_[index];
    }

    const T& operator[](std::size_t index) const noexcept
    {
        assert(index < size_);
        return data_[index];
    }

    template <typename... Args>
    T& EmplaceBack(Args&&... args)
    {
        if (size_ == capacity_)
        {
            Reserve(capacity_ == 0 ? 4 : capacity_ * 2);
        }

        T* entry = MemoryUtil::ConstructAt(data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return *entry;
    }

    void PushBack(const T& value)
    {
        EmplaceBack(value);
    }

    void PushBack(T&& value)
    {
        EmplaceBack(std::move(value));
    }

    void PopBack()
    {
        assert(size_ > 0);
        --size_;
        MemoryUtil::DestroyAt(data_ + size_);
    }

    void Clear() noexcept
    {
        MemoryUtil::DestroyRange(data_, size_);
        size_ = 0;
    }

    void Resize(std::size_t newSize)
    {
        if (newSize < size_)
        {
            for (std::size_t i = newSize; i < size_; ++i)
            {
                MemoryUtil::DestroyAt(data_ + i);
            }
            size_ = newSize;
            return;
        }

        if (newSize > capacity_)
        {
            Reserve(newSize);
        }

        for (std::size_t i = size_; i < newSize; ++i)
        {
            MemoryUtil::ConstructAt(data_ + i);
        }

        size_ = newSize;
    }

    void Reserve(std::size_t requested)
    {
        if (requested <= capacity_)
        {
            return;
        }

        std::size_t nextCapacity = MathUtil::NextPowerOfTwo(requested);
        T* newStorage = MemoryUtil::Allocate<T>(nextCapacity);

        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
        {
            MemoryUtil::UninitializedMove(data_, data_ + size_, newStorage);
        }
        else
        {
            MemoryUtil::UninitializedCopy(data_, data_ + size_, newStorage);
        }

        MemoryUtil::DestroyRange(data_, size_);
        MemoryUtil::Deallocate(data_);

        data_ = newStorage;
        capacity_ = nextCapacity;
    }

private:
    void ReleaseStorage() noexcept
    {
        MemoryUtil::DestroyRange(data_, size_);
        MemoryUtil::Deallocate(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};
} // namespace keyh

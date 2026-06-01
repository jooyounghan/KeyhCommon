#pragma once

#include "Vector.h"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

namespace keyh
{
template <typename T>
class OwnedVector
{
public:
    using Handle = std::unique_ptr<T>;

    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() = default;
        explicit iterator(Handle* current)
            : current_(current)
        {
        }

        reference operator*() const { return *(*current_); }
        pointer operator->() const { return current_->get(); }

        iterator& operator++()
        {
            ++current_;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy(*this);
            ++(*this);
            return copy;
        }

        bool operator==(const iterator&) const = default;

    private:
        Handle* current_ = nullptr;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() = default;
        explicit const_iterator(const Handle* current)
            : current_(current)
        {
        }

        reference operator*() const { return *(*current_); }
        pointer operator->() const { return current_->get(); }

        const_iterator& operator++()
        {
            ++current_;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }

        bool operator==(const const_iterator&) const = default;

    private:
        const Handle* current_ = nullptr;
    };

    OwnedVector() = default;

    [[nodiscard]] std::size_t Size() const noexcept
    {
        return storage_.Size();
    }

    [[nodiscard]] bool Empty() const noexcept
    {
        return storage_.Empty();
    }

    iterator begin() noexcept { return iterator(storage_.Data()); }
    iterator end() noexcept { return iterator(storage_.Data() + storage_.Size()); }
    const_iterator begin() const noexcept { return const_iterator(storage_.Data()); }
    const_iterator end() const noexcept { return const_iterator(storage_.Data() + storage_.Size()); }
    const_iterator cbegin() const noexcept { return const_iterator(storage_.Data()); }
    const_iterator cend() const noexcept { return const_iterator(storage_.Data() + storage_.Size()); }

    T* operator[](std::size_t index) noexcept
    {
        return storage_[index].get();
    }

    const T* operator[](std::size_t index) const noexcept
    {
        return storage_[index].get();
    }

    template <typename... Args>
    T& EmplaceOwned(Args&&... args)
    {
        Handle handle = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *handle;
        storage_.PushBack(std::move(handle));
        return ref;
    }

    void AddOwned(Handle&& handle)
    {
        storage_.PushBack(std::move(handle));
    }

    Handle ReleaseAt(std::size_t index)
    {
        assert(index < storage_.Size());
        Handle released = std::move(storage_[index]);

        for (std::size_t i = index + 1; i < storage_.Size(); ++i)
        {
            storage_[i - 1] = std::move(storage_[i]);
        }

        storage_.PopBack();
        return released;
    }

    void Clear()
    {
        storage_.Clear();
    }

private:
    Vector<Handle> storage_;
};
} // namespace keyh

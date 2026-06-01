#pragma once

#include "HashMap.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <utility>

namespace keyh
{
template <typename K, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashSet
{
public:
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = K;
        using difference_type = std::ptrdiff_t;
        using reference = const K&;

        iterator() = default;
        explicit iterator(typename HashMap<K, std::uint8_t, Hasher, KeyEqual>::iterator current)
            : current_(current)
        {
        }

        reference operator*() const
        {
            return (*current_).first;
        }

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
        typename HashMap<K, std::uint8_t, Hasher, KeyEqual>::iterator current_;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = K;
        using difference_type = std::ptrdiff_t;
        using reference = const K&;

        const_iterator() = default;
        explicit const_iterator(typename HashMap<K, std::uint8_t, Hasher, KeyEqual>::const_iterator current)
            : current_(current)
        {
        }

        reference operator*() const
        {
            return (*current_).first;
        }

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
        typename HashMap<K, std::uint8_t, Hasher, KeyEqual>::const_iterator current_;
    };

    bool Insert(const K& key)
    {
        return map_.Insert(key, kDummyValue);
    }

    bool Remove(const K& key)
    {
        return map_.Remove(key);
    }

    [[nodiscard]] bool Contains(const K& key) const
    {
        return map_.Contains(key);
    }

    [[nodiscard]] std::size_t Size() const noexcept
    {
        return map_.Size();
    }

    [[nodiscard]] bool Empty() const noexcept
    {
        return map_.Empty();
    }

    iterator begin() noexcept { return iterator(map_.begin()); }
    iterator end() noexcept { return iterator(map_.end()); }
    const_iterator begin() const noexcept { return const_iterator(map_.begin()); }
    const_iterator end() const noexcept { return const_iterator(map_.end()); }
    const_iterator cbegin() const noexcept { return const_iterator(map_.cbegin()); }
    const_iterator cend() const noexcept { return const_iterator(map_.cend()); }

    void Clear()
    {
        map_.Clear();
    }

private:
    static constexpr std::uint8_t kDummyValue = 0;
    HashMap<K, std::uint8_t, Hasher, KeyEqual> map_;
};
} // namespace keyh

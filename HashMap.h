#pragma once

#include "MathUtil.h"
#include "Vector.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

namespace keyh
{
template <typename K, typename V, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashMap
{
public:
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const K&, V&>;
        using difference_type = std::ptrdiff_t;

        iterator() = default;
        iterator(HashMap* map, std::size_t index)
            : map_(map), index_(index)
        {
            SkipUnoccupied();
        }

        value_type operator*() const
        {
            Bucket& bucket = map_->buckets_[index_];
            return { bucket.key, bucket.value };
        }

        iterator& operator++()
        {
            ++index_;
            SkipUnoccupied();
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
        void SkipUnoccupied()
        {
            while (index_ < map_->buckets_.Size() && !map_->buckets_[index_].occupied)
            {
                ++index_;
            }
        }

        HashMap* map_ = nullptr;
        std::size_t index_ = 0;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const K&, const V&>;
        using difference_type = std::ptrdiff_t;

        const_iterator() = default;
        const_iterator(const HashMap* map, std::size_t index)
            : map_(map), index_(index)
        {
            SkipUnoccupied();
        }

        value_type operator*() const
        {
            const Bucket& bucket = map_->buckets_[index_];
            return { bucket.key, bucket.value };
        }

        const_iterator& operator++()
        {
            ++index_;
            SkipUnoccupied();
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
        void SkipUnoccupied()
        {
            while (index_ < map_->buckets_.Size() && !map_->buckets_[index_].occupied)
            {
                ++index_;
            }
        }

        const HashMap* map_ = nullptr;
        std::size_t index_ = 0;
    };

    HashMap()
    {
        Rehash(8);
    }

    [[nodiscard]] std::size_t Size() const noexcept { return size_; }
    [[nodiscard]] bool Empty() const noexcept { return size_ == 0; }

    iterator begin() noexcept { return iterator(this, 0); }
    iterator end() noexcept { return iterator(this, buckets_.Size()); }
    const_iterator begin() const noexcept { return const_iterator(this, 0); }
    const_iterator end() const noexcept { return const_iterator(this, buckets_.Size()); }
    const_iterator cbegin() const noexcept { return const_iterator(this, 0); }
    const_iterator cend() const noexcept { return const_iterator(this, buckets_.Size()); }

    void Clear()
    {
        for (Bucket& bucket : buckets_)
        {
            bucket.occupied = false;
            bucket.distance = 0;
        }
        size_ = 0;
    }

    bool Insert(const K& key, const V& value)
    {
        return Emplace(key, value).second;
    }

    template <typename KeyArg, typename ValueArg>
    std::pair<V*, bool> Emplace(KeyArg&& key, ValueArg&& value)
    {
        EnsureCapacity();

        K insertKey = std::forward<KeyArg>(key);
        if (V* existing = Find(insertKey))
        {
            return { existing, false };
        }

        V insertValue = std::forward<ValueArg>(value);
        std::size_t hash = hasher_(insertKey);
        std::size_t index = hash & Mask();
        std::size_t distance = 0;

        while (true)
        {
            Bucket& bucket = buckets_[index];
            if (!bucket.occupied)
            {
                bucket.hash = hash;
                bucket.distance = distance;
                bucket.occupied = true;
                bucket.key = std::move(insertKey);
                bucket.value = std::move(insertValue);
                ++size_;
                return { &bucket.value, true };
            }

            if (bucket.distance < distance)
            {
                std::swap(hash, bucket.hash);
                std::swap(distance, bucket.distance);
                std::swap(insertKey, bucket.key);
                std::swap(insertValue, bucket.value);
            }

            index = (index + 1) & Mask();
            ++distance;
        }
    }

    V* Find(const K& key)
    {
        std::size_t hash = hasher_(key);
        std::size_t index = hash & Mask();
        std::size_t distance = 0;

        while (true)
        {
            Bucket& bucket = buckets_[index];
            if (!bucket.occupied)
            {
                return nullptr;
            }

            if (bucket.distance < distance)
            {
                return nullptr;
            }

            if (bucket.hash == hash && equal_(bucket.key, key))
            {
                return &bucket.value;
            }

            index = (index + 1) & Mask();
            ++distance;
        }
    }

    const V* Find(const K& key) const
    {
        return const_cast<HashMap*>(this)->Find(key);
    }

    bool Contains(const K& key) const
    {
        return Find(key) != nullptr;
    }

    V& operator[](const K& key)
    {
        auto [value, wasInserted] = Emplace(key, V{});
        (void)wasInserted;
        return *value;
    }

    bool Remove(const K& key)
    {
        std::size_t hash = hasher_(key);
        std::size_t index = hash & Mask();
        std::size_t distance = 0;

        while (true)
        {
            Bucket& bucket = buckets_[index];
            if (!bucket.occupied)
            {
                return false;
            }

            if (bucket.distance < distance)
            {
                return false;
            }

            if (bucket.hash == hash && equal_(bucket.key, key))
            {
                RemoveAt(index);
                --size_;
                return true;
            }

            index = (index + 1) & Mask();
            ++distance;
        }
    }

private:
    struct Bucket
    {
        K key{};
        V value{};
        std::size_t hash = 0;
        std::size_t distance = 0;
        bool occupied = false;
    };

    [[nodiscard]] std::size_t Mask() const noexcept
    {
        return buckets_.Size() - 1;
    }

    void EnsureCapacity()
    {
        constexpr float kMaxLoadFactor = 0.7f;
        if (static_cast<float>(size_ + 1) / static_cast<float>(buckets_.Size()) > kMaxLoadFactor)
        {
            Rehash(buckets_.Size() * 2);
        }
    }

    void Rehash(std::size_t newCapacity)
    {
        newCapacity = MathUtil::NextPowerOfTwo(MathUtil::Max<std::size_t>(8, newCapacity));

        Vector<Bucket> previous = std::move(buckets_);
        buckets_.Reserve(newCapacity);
        buckets_.Resize(newCapacity);

        std::size_t previousSize = size_;
        size_ = 0;

        for (std::size_t i = 0; i < previous.Size(); ++i)
        {
            if (previous[i].occupied)
            {
                Emplace(std::move(previous[i].key), std::move(previous[i].value));
            }
        }

        assert(size_ == previousSize);
    }

    void RemoveAt(std::size_t index)
    {
        std::size_t current = index;
        std::size_t next = (current + 1) & Mask();

        while (buckets_[next].occupied && buckets_[next].distance > 0)
        {
            buckets_[current] = std::move(buckets_[next]);
            --buckets_[current].distance;

            current = next;
            next = (next + 1) & Mask();
        }

        buckets_[current].occupied = false;
        buckets_[current].distance = 0;
    }

    Vector<Bucket> buckets_;
    std::size_t size_ = 0;
    Hasher hasher_{};
    KeyEqual equal_{};
};
} // namespace keyh

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace KE
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

template <typename T>
class OwnedVector
{
public:
    using Handle = std::unique_ptr<T>;

    OwnedVector() = default;

    [[nodiscard]] std::size_t Size() const noexcept
    {
        return storage_.Size();
    }

    [[nodiscard]] bool Empty() const noexcept
    {
        return storage_.Empty();
    }

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

template <typename T, std::size_t N>
class StaticVector
{
public:
    StaticVector() = default;

    [[nodiscard]] constexpr std::size_t Size() const noexcept { return size_; }
    [[nodiscard]] constexpr std::size_t Capacity() const noexcept { return N; }
    [[nodiscard]] constexpr bool Empty() const noexcept { return size_ == 0; }

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

template <typename K, typename V, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashMap
{
public:
    HashMap()
    {
        Rehash(8);
    }

    [[nodiscard]] std::size_t Size() const noexcept { return size_; }
    [[nodiscard]] bool Empty() const noexcept { return size_ == 0; }

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

        K insertKey = key;
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

template <typename K, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashSet
{
public:
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

    void Clear()
    {
        map_.Clear();
    }

private:
    static constexpr std::uint8_t kDummyValue = 0;
    HashMap<K, std::uint8_t, Hasher, KeyEqual> map_;
};
} // namespace KE

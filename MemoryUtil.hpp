namespace keyh
{
    template<typename T>
    constexpr T* MemoryUtil::addressOf(T& arg) noexcept
    {
        return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(arg)
            )
        );
    }

    template<typename T>
    constexpr T MemoryUtil::align(T input, TypeIdentity<T> alignedValue) noexcept
    {
        TypeTrait::requireIntegral<T>();
        return (input + (alignedValue - 1)) & ~(alignedValue - 1);
    }

    template<typename T>
    constexpr bool MemoryUtil::hasSingleBit(T value) noexcept
    {
        TypeTrait::requireIntegral<T>();
        return (value & (value - 1)) == 0;
    }

    template<typename T>
    constexpr T MemoryUtil::bitCeil(T value) noexcept
    {
        TypeTrait::requireIntegral<T>();
        T power = 1;
        while (power < value) power <<= 1;
        return power;
    }

    template <typename T1, typename... Types>
    constexpr size_t MemoryUtil::alignOf() noexcept
    {
        if constexpr (sizeof...(Types) == 0)
        {
            return alignof(T1);
        }
        else
        {
            constexpr size_t head = alignOf<T1>();
            constexpr size_t tail = alignOf<Types...>();
            return head > tail ? head : tail;
        }
    }

    template<size_t ColumnIndex, typename T, typename... Types>
    struct GetType
    {
        using Type = typename GetType<ColumnIndex - 1, Types...>::Type;
    };

    template<typename T, typename... Types>
    struct GetType<0, T, Types...>
    {
        using Type = T;
    };

    template<size_t ColumnIndex, typename ...Types>
    constexpr size_t MemoryUtil::getOffset() noexcept
    {
        if constexpr (ColumnIndex == 0)
        {
            return 0;
        }
        else
        {
            static_assert(ColumnIndex < sizeof...(Types), "Index out of bounds for Types");

            using CurrentType = typename GetType<ColumnIndex, Types...>::Type;
            using PrevType = typename GetType<ColumnIndex - 1, Types...>::Type;

            constexpr size_t prevSize = sizeof(PrevType);
            constexpr size_t prevOffset = MemoryUtil::getOffset<ColumnIndex - 1, Types...>();

            constexpr size_t currentOffset = prevOffset + prevSize;
            return MemoryUtil::align(currentOffset, alignof(CurrentType));
        }
    }

    template<typename T>
    constexpr size_t MemoryUtil::getPaddedSizeOfImpl() noexcept
    {
        return sizeof(T);
    }

    template<typename T1, typename T2, typename ...Types>
    constexpr size_t MemoryUtil::getPaddedSizeOfImpl() noexcept
    {
        return MemoryUtil::align(getPaddedSizeOfImpl<T2, Types...>(), alignof(T2)) + getPaddedSizeOfImpl<T1>();
    }


    template <typename ...Types>
    constexpr size_t MemoryUtil::getPaddedSizeOf() noexcept
    {
        constexpr size_t maxAlignment = alignOf<Types...>();
        return MemoryUtil::align(getPaddedSizeOfImpl<Types...>(), maxAlignment);
    }

    template<typename T, typename ...Types>
    constexpr size_t MemoryUtil::getSizeOf() noexcept
    {
        if constexpr (sizeof...(Types) == 0)
        {
            return sizeof(T);
        }
        else
        {
            return sizeof(T) + getSizeOf<Types...>();
        }
    }

    template <bool InitializeNull, typename ...Types>
    void* MemoryUtil::alignedMalloc(size_t count)
    {
        if (count == 0) return nullptr;

        constexpr size_t maxAlignment = alignOf<Types...>();
        size_t requestedBytes = count * getPaddedSizeOf<Types...>();

        if constexpr (maxAlignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__ && requestedBytes >= kPageThresholdSize)
        {
            maxAlignment = maxAlignment > kCachelineAlignSize ? maxAlignment : kCachelineAlignSize;
        }

        void* ptr = _aligned_malloc(requestedBytes, maxAlignment);

        if constexpr (InitializeNull)
        {
            if (ptr != nullptr)
            {
                memset(ptr, 0, requestedBytes);
            }
        }
        return ptr;
    }

    template<typename ...Types>
    void MemoryUtil::alignedFree(void* ptr)
    {
        return _aligned_free(ptr);
    }

    template<typename T>
    void MemoryUtil::safeRelease(T*& ptr) noexcept
    {
        if (ptr != nullptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    template<typename T>
    void keyh::MemoryUtil::swap(T& a, T& b) noexcept
    {
		T temp = move(a);
		a = move(b);
		b = move(temp);
    }
}
#pragma once
#include "Vector.h"
#include "StaticArray.h"

namespace keyh
{
    template <typename T>
    class Span
    {
    private:
        const T* _data;
        size_t _size;

    public:
		Span();
        Span(const T* data, size_t size);
        template <size_t Size>
        Span(const StaticArray<T, Size>& arr);
        Span(const Vector<T>& vec);

    public:
		Span(const Span<T>& other);

    public:
        inline const T* data() const { return _data; }
        inline size_t size() const { return _size; }

    public:
        inline const T& operator[](size_t index) const { return _data[index]; }
        inline const T* begin() const { return _data; }
        inline const T* end() const { return _data + _size; }
    };
}
#include "Span.hpp"


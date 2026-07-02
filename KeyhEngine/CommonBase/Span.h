#pragma once
#include "Vector.h"
#include "StaticArray.h"

namespace keyh
{
    template <typename T>
    class Span
    {
    private:
        T* _data;
        size_t _size;

    public:
        Span(T* data, size_t size);
        template <size_t Size>
        Span(const StaticArray<T, Size>& arr);
        Span(const Vector<T>& vec);

    public:
        inline T* data() const { return _data; }
        inline size_t size() const { return _size; }

    public:
        inline T& operator[](size_t index) const { return _data[index]; }
        inline T* begin() const { return _data; }
        inline T* end() const { return _data + _size; }
    };
}
#include "Span.hpp"


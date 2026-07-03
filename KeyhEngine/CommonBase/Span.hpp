#include "Span.h"
namespace keyh
{
    template<typename T>
    Span<T>::Span() : _data(nullptr), _size(0) 
    {}

    template<typename T>
    Span<T>::Span(const T* data, size_t size)
        : _data(data), _size(size)
    {}
    
    template<typename T>
    Span<T>::Span(const Vector<T>& vec)
        : _data(vec.data()), _size(vec.size())
    {}

    template<typename T>
    Span<T>::Span(const Span<T>&other)
		: _data(other._data), _size(other._size)
    {}
    
    template<typename T>
    template<size_t Size>
    Span<T>::Span(const StaticArray<T, Size>& arr)
        : _data(arr.data()), _size(arr.size())
    {}
}
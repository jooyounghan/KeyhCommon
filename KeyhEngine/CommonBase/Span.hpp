namespace keyh
{
    template<typename T>
    Span<T>::Span(T* data, size_t size)
        : _data(data), _size(size)
    {}
    
    template<typename T>
    Span<T>::Span(const Vector<T>& vec)
        : _data(vec.data()), _size(vec.size())
    {}
    
    template<typename T>
    template<size_t Size>
    Span<T>::Span(const StaticArray<T, Size>& arr)
        : _data(arr.data()), _size(arr.size())
    {}
}
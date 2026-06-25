namespace keyh
{
	template<typename T, typename Derived>
	void IBuffer<T, Derived>::write(const void* input, size_t size)
	{
		if (getAvailableSize() < size)
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write data");
			return;
		}

		KEYH_ASSERT_DEV(size % sizeof(T) == 0, "Size must be a multiple of element size");
		T* buffer = getBuffer();
		memcpy(static_cast<void*>(buffer + _offset), input, size);
		_offset += (size / sizeof(T));
		buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBuffer<T, Derived>::writeOne(T input)
	{
		if (getAvailableSize() == 0)
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write data");
			return;
		}

		T* buffer = getBuffer();
		buffer[_offset] = input;
		_offset++;
		buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBuffer<T, Derived>::reset()
	{
		_offset = 0;
		getDerived()->resetImpl();
	}

	template<typename T, typename Derived>
	size_t IBuffer<T, Derived>::getAvailableSize() const
	{
		const size_t capacity = getDerived()->getCapacityImpl();
		return capacity > size() ? capacity - size() : 0;
	}
}
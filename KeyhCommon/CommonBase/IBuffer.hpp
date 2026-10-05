namespace keyh
{
	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::write(const void* input, size_t size)
	{
		if (getAvailableSize() < size)
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write data");
			return;
		}

		if (size % sizeof(T) != 0)
		{
			KEYH_ASSERT(false, "Write size must be a multiple of the buffer element size.");
			return;
		}
		if (size == 0)
			return;
		T* buffer = getBuffer();
		memcpy(static_cast<void*>(buffer + _offset), input, size);
		_offset += (size / sizeof(T));
		if constexpr (Derived::kIsString)
			buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::writeOne(T input)
	{
		if (getAvailableSize() < sizeof(T))
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write data");
			return;
		}

		T* buffer = getBuffer();
		buffer[_offset] = input;
		_offset++;
		if constexpr (Derived::kIsString)
			buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::reset()
	{
		_offset = 0;
		getDerived()->resetImpl();
	}

	template<typename T, typename Derived>
	size_t IBufferBase<T, Derived>::getAvailableSize() const
	{
		const size_t writableCapacity = getDerived()->getWritableCapacityImpl();
		return writableCapacity > size() ? writableCapacity - size() : 0;
	}
}

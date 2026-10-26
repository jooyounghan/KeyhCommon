namespace keyh
{
	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::write(const void* input, size_t count)
	{
		if (getAvailableSize() < count)
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write data");
			return;
		}

		if (count == 0)
			return;
		T* buffer = getBuffer();
		memcpy(static_cast<void*>(buffer + _offset), input, count * sizeof(T));
		_offset += count;
		if constexpr (Derived::kIsString)
			buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::writeBytes(const void* input, size_t size)
	{
		if (size == 0)
			return;
		const size_t elementCount = size / sizeof(T) + (size % sizeof(T) != 0 ? 1 : 0);
		if (getAvailableSize() < elementCount)
		{
			KEYH_ASSERT(false, "Not enough space in buffer to write the requested bytes.");
			return;
		}
		T* buffer = getBuffer();
		memcpy(static_cast<void*>(reinterpret_cast<char*>(buffer) + _offset * sizeof(T)), input, size);
		_offset += elementCount;
		if constexpr (Derived::kIsString)
			buffer[_offset] = T();
	}

	template<typename T, typename Derived>
	void IBufferBase<T, Derived>::writeOne(T input)
	{
		if (getAvailableSize() < 1)
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

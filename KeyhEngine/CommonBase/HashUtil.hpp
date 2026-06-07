namespace keyh
{
	template<typename Key>
	size_t DefaultHasher::hash(const Key& key) noexcept
	{
		size_t hash = DefaultHasher::kInitialHashValue;
		const byte* data = reinterpret_cast<const byte*>(MemoryUtil::addressOf(const_cast<Key&>(key)));

		const size_t kSizeOfSizeT = sizeof(size_t);
		size_t blocks = sizeof(Key) / kSizeOfSizeT;
		size_t remainder = sizeof(Key) % kSizeOfSizeT;

		for (size_t i = 0; i < blocks; ++i)
		{
			size_t blockValue;
			std::memcpy(&blockValue, data, kSizeOfSizeT);

			hash ^= blockValue;
			hash *= DefaultHasher::kHashPrime;

			data += kSizeOfSizeT;
		}

		for (size_t i = 0; i < remainder; ++i)
		{
			hash ^= static_cast<size_t>(static_cast<unsigned char>(data[i]));
			hash *= DefaultHasher::kHashPrime;
		}

		return hash;
	}

	template<typename T>
	size_t Hash<T>::operator()(const T& value) const noexcept
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(Hash<T>);
	}
}
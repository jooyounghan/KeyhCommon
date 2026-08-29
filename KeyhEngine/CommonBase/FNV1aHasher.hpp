
#define DEFINE_FNV1A_HASH_DEFAULT_SPECIALIZATION(Type) template<> size_t FNV1aHash<Type>::operator()(const Type& value) const noexcept { return FNV1aHasher::hash(value); }

namespace keyh
{
	template<typename Key>
	size_t FNV1aHasher::hash(const Key& key) noexcept
	{
		size_t hash = FNV1aHasher::kInitialHashValue;
		const byte* data = reinterpret_cast<const byte*>(MemoryUtil::addressOf(const_cast<Key&>(key)));

		const size_t kSizeOfSizeT = sizeof(size_t);
		size_t blocks = sizeof(Key) / kSizeOfSizeT;
		size_t remainder = sizeof(Key) % kSizeOfSizeT;

		for (size_t i = 0; i < blocks; ++i)
		{
			size_t blockValue;
			memcpy(&blockValue, data, kSizeOfSizeT);

			hash ^= blockValue;
			hash *= FNV1aHasher::kHashPrime;

			data += kSizeOfSizeT;
		}

		for (size_t i = 0; i < remainder; ++i)
		{
			hash ^= static_cast<size_t>(static_cast<unsigned char>(data[i]));
			hash *= FNV1aHasher::kHashPrime;
		}

		return hash;
	}

	template<typename T>
	size_t FNV1aHash<T>::operator()(const T& value) const noexcept
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
	}
}
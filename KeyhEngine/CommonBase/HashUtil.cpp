#include "CommonBasePch.h"
#include "HashUtil.h"

namespace keyh
{
	size_t DefaultHasher::hash(const char* str, size_t length) noexcept
	{
		size_t hashValue = DefaultHasher::kInitialHashValue;
		const size_t kSizeOfSizeT = sizeof(size_t);

		size_t blocks = length / kSizeOfSizeT;
		size_t remainder = length % kSizeOfSizeT;

		const char* ptr = str;

		for (size_t i = 0; i < blocks; ++i)
		{
			size_t blockValue;
			std::memcpy(&blockValue, ptr, kSizeOfSizeT);

			hashValue ^= blockValue;
			hashValue *= DefaultHasher::kHashPrime;

			ptr += kSizeOfSizeT;
		}

		for (size_t i = 0; i < remainder; ++i)
		{
			hashValue ^= static_cast<size_t>(static_cast<unsigned char>(ptr[i]));
			hashValue *= DefaultHasher::kHashPrime;
		}

		return hashValue;
	}

#define DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hasher, Raw)			\
template<>															\
size_t Hasher<Raw>::operator()(const Raw& value) const noexcept		\
{																	\
	return DefaultHasher::hash(value);								\
}

	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, bool);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint64);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint32);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint16);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint8);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int64);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int32);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int16);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int8);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, double);
	DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Hash, float);

#undef DEFINE_RAW_VALUE_HASH_SPECIALIZATION
}
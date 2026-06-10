#include "CommonBasePch.h"
#include "FNV1aHasher.h"

namespace keyh
{
	size_t FNV1aHasher::hash(const char* str, size_t length) noexcept
	{
		size_t hashValue = FNV1aHasher::kInitialHashValue;
		const size_t kSizeOfSizeT = sizeof(size_t);

		size_t blocks = length / kSizeOfSizeT;
		size_t remainder = length % kSizeOfSizeT;

		const char* ptr = str;

		for (size_t i = 0; i < blocks; ++i)
		{
			size_t blockValue;
			std::memcpy(&blockValue, ptr, kSizeOfSizeT);

			hashValue ^= blockValue;
			hashValue *= FNV1aHasher::kHashPrime;

			ptr += kSizeOfSizeT;
		}

		for (size_t i = 0; i < remainder; ++i)
		{
			hashValue ^= static_cast<size_t>(static_cast<unsigned char>(ptr[i]));
			hashValue *= FNV1aHasher::kHashPrime;
		}

		return hashValue;
	}

#define DEFINE_RAW_VALUE_HASH_SPECIALIZATION(Raw)					\
template<>															\
size_t FNV1aHash<Raw>::operator()(const Raw& value) const noexcept	\
{																	\
	return FNV1aHasher::hash(value);								\
}

DEFINE_RAW_VALUE_HASH_SPECIALIZATION(bool);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(uint64);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(uint32);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(uint16);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(uint8);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(int64);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(int32);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(int16);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(int8);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(double);
DEFINE_RAW_VALUE_HASH_SPECIALIZATION(float);

#undef DEFINE_RAW_VALUE_HASH_SPECIALIZATION
}
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

	size_t FNV1aHasher::hash(const wchar_t* str, size_t length) noexcept
	{
		size_t hashValue = FNV1aHasher::kInitialHashValue;
		const size_t kSizeOfSizeT = sizeof(size_t);
		const size_t kSizeOfWChar = sizeof(wchar_t);

		size_t totalBytes = length * kSizeOfWChar;
		size_t blocks = totalBytes / kSizeOfSizeT;
		size_t remainder = totalBytes % kSizeOfSizeT;

		const char* ptr = reinterpret_cast<const char*>(str);

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

#if defined(SIMD_ENABLED)
	size_t FNV1aHasher::hashSimd(const char* str, size_t length) noexcept
	{
		static constexpr size_t kLanes = SimdUtil::kSimdTargetByte / sizeof(size_t);
		static constexpr size_t kBlock = SimdUtil::kSimdTargetByte;

		size_t h[kLanes];
		for (size_t i = 0; i < kLanes; ++i)
			h[i] = kInitialHashValue;

		const char* ptr = str;
		const char* end = str + length;
		bool anySimdIter = false;

		alignas(SimdUtil::kSimdTargetByte) size_t lanes[kLanes];
		SimdUtil simd;

		while (static_cast<size_t>(end - ptr) >= kBlock)
		{
			SimdUtil::SimdVec data = simd.load(ptr);
			simd.storeTo(lanes, data);
			for (size_t i = 0; i < kLanes; ++i)
			{
				h[i] ^= lanes[i];
				h[i] *= kHashPrime;
			}
			ptr += kBlock;
			anySimdIter = true;
		}

		size_t hashValue = h[0];
		if (anySimdIter)
		{
			for (size_t i = 1; i < kLanes; ++i)
				hashValue = (hashValue ^ h[i]) * kHashPrime;
		}

		const size_t kSizeOfSizeT = sizeof(size_t);
		while (static_cast<size_t>(end - ptr) >= kSizeOfSizeT)
		{
			size_t blockValue;
			std::memcpy(&blockValue, ptr, kSizeOfSizeT);
			hashValue ^= blockValue;
			hashValue *= kHashPrime;
			ptr += kSizeOfSizeT;
		}

		for (; ptr < end; ++ptr)
		{
			hashValue ^= static_cast<size_t>(static_cast<unsigned char>(*ptr));
			hashValue *= kHashPrime;
		}

		return hashValue;
	}

	size_t FNV1aHasher::hashSimd(const wchar_t* str, size_t length) noexcept
	{
		return hashSimd(reinterpret_cast<const char*>(str), length * sizeof(wchar_t));
	}
#endif
}
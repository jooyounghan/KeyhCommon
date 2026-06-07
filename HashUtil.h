#pragma once
#include "AssertUtil.h"
#include "MemoryUtil.h"

#define DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hasher, Raw) template<> size_t Hasher<Raw>::operator()(const Raw& value) const noexcept
#define DECLARE_TEMPLATED_HASH_SPECIALIZATION(Hasher,Class, ...) template<> size_t Hasher<Class<__VA_ARGS__>>::operator()(const Class<__VA_ARGS__>& value) const noexcept

namespace keyh
{
	struct DefaultHasher
	{
		constexpr static size_t kInitialHashValue = 14695981039346656037ULL;
		constexpr static size_t kHashPrime = 1099511628211ULL;

		static size_t hash(const char* str, size_t length) noexcept;

		template<typename Key>
		static size_t hash(const Key& key) noexcept;
	};

	template<typename T>
	struct Hash
	{
		size_t operator()(const T& value) const noexcept;
	};

    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, bool);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint64);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint32);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint16);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, uint8);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int64);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int32);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int16);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, int8);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, double);
    DECLARE_RAW_VALUE_HASH_SPECIALIZATION(Hash, float);
}
#include "HashUtil.hpp"
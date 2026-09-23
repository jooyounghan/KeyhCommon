#pragma once
#include "CommonCore.h"
#include "AssertUtil.h"
#include "MemoryUtil.h"

#define DECLARE_FNV1A_HASH_SPECIALIZATION(Type) template<> size_t FNV1aHash<Type>::operator()(const Type& value) const noexcept
#define DECLARE_TEMPLATED_FNV1A_HASH_SPECIALIZATION(Class, ...) template<> size_t FNV1aHash<Class<__VA_ARGS__>>::operator()(const Class<__VA_ARGS__>& value) const noexcept

namespace keyh
{
	struct FNV1aHasher
	{
		static constexpr size_t kInitialHashValue = 14695981039346656037ULL;
		static constexpr size_t kHashPrime = 1099511628211ULL;

		static size_t hash(const char* str, size_t length) noexcept;
		static size_t hash(const wchar_t* str, size_t length) noexcept;

		template<typename Key>
		static size_t hash(const Key& key) noexcept;
	};

	template<typename T>
	struct FNV1aHash
	{
		size_t operator()(const T& value) const noexcept;
	};

	DECLARE_FNV1A_HASH_SPECIALIZATION(bool);
	DECLARE_FNV1A_HASH_SPECIALIZATION(uint64);
	DECLARE_FNV1A_HASH_SPECIALIZATION(uint32);
	DECLARE_FNV1A_HASH_SPECIALIZATION(uint16);
	DECLARE_FNV1A_HASH_SPECIALIZATION(uint8);
	DECLARE_FNV1A_HASH_SPECIALIZATION(int64);
	DECLARE_FNV1A_HASH_SPECIALIZATION(int32);
	DECLARE_FNV1A_HASH_SPECIALIZATION(int16);
	DECLARE_FNV1A_HASH_SPECIALIZATION(int8);
	DECLARE_FNV1A_HASH_SPECIALIZATION(double);
	DECLARE_FNV1A_HASH_SPECIALIZATION(float);
}

#include "FNV1aHasher.hpp"
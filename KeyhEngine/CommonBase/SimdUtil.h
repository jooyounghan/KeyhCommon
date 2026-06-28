#pragma once

#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include <immintrin.h>
#define KEYH_X86_SIMD
#endif

#if defined(KEYH_X86_SIMD) && (defined(__AVX512F__) || defined(__AVX2__))
#define SIMD_ENABLED
#endif

#if defined(__AVX512F__)
// =============================== AVX512 =============================
#define SIMD_ALL_ONES_MASK  (0xFFFFFFFFFFFFFFFFULL)

#if defined(_MSC_VER)
#define SIMD_MASK_BSF(idx, mask)  _BitScanForward64(&(idx), (mask))
#else
#define SIMD_MASK_BSF(idx, mask)  (idx = __builtin_ctzll(mask))
#endif
// ====================================================================

#elif defined(__AVX2__)
// =============================== AVX2 ===============================
#define SIMD_ALL_ONES_MASK	(0xFFFFFFFFu)

#if defined(_MSC_VER)
#define SIMD_MASK_BSF(idx, mask)  _BitScanForward(&(idx), (mask))
#else
#define SIMD_MASK_BSF(idx, mask)  (idx = __builtin_ctz(mask))
#endif
// ====================================================================
#endif

struct SimdUtil
{
#if defined(SIMD_ENABLED)	

	static constexpr size_t kSimdTargetByte 
#if defined(__AVX512F__)
		= 64;
#elif defined(__AVX2__)
		= 32;
#endif

	using SimdMask 
#if defined(__AVX512F__)
		= unsigned long long;
#elif defined(__AVX2__)
		= unsigned int;
#endif

	using SimdVec 
#if defined(__AVX512F__)
		= __m512i;
#elif defined(__AVX2__)
		= __m256i;
#endif

	[[nodiscard]]
	inline SimdVec set8(char x) noexcept
	{
#if defined(__AVX512F__)
		return _mm512_set1_epi8(x);
#elif defined(__AVX2__)
		return _mm256_set1_epi8(x);
#endif
	}

	[[nodiscard]]
	inline SimdVec load(const void* p) noexcept
	{
#if defined(__AVX512F__)
		return _mm512_loadu_si512(reinterpret_cast<const __m512i*>(p));
#elif defined(__AVX2__)
		return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(p));
#endif
	}

	[[nodiscard]]
	inline SimdMask cmpEpi8(SimdVec a, SimdVec b) noexcept
	{
#if defined(__AVX512F__)
		return static_cast<SimdMask>(_mm512_cmpeq_epi8_mask(a, b));
#elif defined(__AVX2__)
		return static_cast<SimdMask>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b)));
#endif
	}

	[[nodiscard]]
	inline unsigned int bitScanForward(SimdMask mask) noexcept
	{
		unsigned long index;
		SIMD_MASK_BSF(index, mask);
		return static_cast<unsigned int>(index);
	}

	[[nodiscard]]
	inline SimdVec xorVec(SimdVec a, SimdVec b) noexcept
	{
#if defined(__AVX512F__)
		return _mm512_xor_si512(a, b);
#elif defined(__AVX2__)
		return _mm256_xor_si256(a, b);
#endif
	}

	inline void storeTo(void* dest, SimdVec v) noexcept
	{
#if defined(__AVX512F__)
		_mm512_storeu_si512(reinterpret_cast<__m512i*>(dest), v);
#elif defined(__AVX2__)
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(dest), v);
#endif
	}

#endif
};





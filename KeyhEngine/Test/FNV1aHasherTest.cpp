#include "TestCommon.h"
#include "FNV1aHasherTest.h"
#include "FNV1aHasher.h"
#include <chrono>
#include <cstdio>
#include <cstring>

using namespace keyh;

// ============================================================
//  Correctness
// ============================================================

void test_FNV1aHasher_simd_correctness()
{
    printSection("FNV1aHasher - SIMD Correctness");

    // Short string: shorter than one SIMD block (< 32 bytes)
    // → SIMD skips the parallel loop entirely and must equal scalar.
    const char* shortStr = "hello";
    const size_t shortLen = 5;

    size_t scalarShort = FNV1aHasher::hash(shortStr, shortLen);
    size_t scalarShort2 = FNV1aHasher::hash(shortStr, shortLen);
    CHECK(scalarShort == scalarShort2);  // scalar is deterministic

#if defined(SIMD_ENABLED)
    size_t simdShort = FNV1aHasher::hashSimd(shortStr, shortLen);
    size_t simdShort2 = FNV1aHasher::hashSimd(shortStr, shortLen);
    CHECK(simdShort == simdShort2);                // SIMD is deterministic
    CHECK(simdShort == scalarShort);               // short path: must match scalar

    // Empty string
    CHECK(FNV1aHasher::hashSimd("", 0) == FNV1aHasher::hash("", 0));

    // Exactly one SIMD block (32 bytes for AVX2): SIMD and scalar differ
    // but each must be self-consistent.
    const char block32[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ012345";
    size_t simdBlock = FNV1aHasher::hashSimd(block32, 32);
    CHECK(simdBlock == FNV1aHasher::hashSimd(block32, 32));  // deterministic

    // Long string: multiple SIMD blocks
    static char longBuf[4096];
    for (size_t i = 0; i < sizeof(longBuf); ++i)
        longBuf[i] = static_cast<char>('a' + (i % 26));
    size_t simdLong = FNV1aHasher::hashSimd(longBuf, sizeof(longBuf));
    CHECK(simdLong == FNV1aHasher::hashSimd(longBuf, sizeof(longBuf)));  // deterministic

    // Different content → different hash (basic collision check)
    static char longBuf2[4096];
    for (size_t i = 0; i < sizeof(longBuf2); ++i)
        longBuf2[i] = static_cast<char>('z' - (i % 26));
    CHECK(simdLong != FNV1aHasher::hashSimd(longBuf2, sizeof(longBuf2)));
#else
    std::printf("  [INFO] SIMD_ENABLED not defined – SIMD tests skipped\n");
#endif
}

// ============================================================
//  Benchmark
// ============================================================

void bench_FNV1aHasher_scalar_vs_simd()
{
    printSection("FNV1aHasher - Scalar vs SIMD Benchmark");

    static constexpr size_t kDataSize  = 1024 * 1024;   // 1 MB
    static constexpr int    kIter      = 2000;

    static char data[kDataSize];
    for (size_t i = 0; i < kDataSize; ++i)
        data[i] = static_cast<char>('a' + (i % 26));

    // ---- Scalar ----
    volatile size_t sink = 0;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kIter; ++i)
        sink ^= FNV1aHasher::hash(data, kDataSize);
    auto t1 = std::chrono::high_resolution_clock::now();

    long long scalarMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::printf("  Scalar : %4lld ms  (result prefix: 0x%08llx)\n",
        scalarMs,
        static_cast<unsigned long long>(sink & 0xFFFFFFFFull));

#if defined(SIMD_ENABLED)
    // ---- SIMD ----
    sink = 0;

    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kIter; ++i)
        sink ^= FNV1aHasher::hashSimd(data, kDataSize);
    auto t3 = std::chrono::high_resolution_clock::now();

    long long simdMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

    std::printf("  SIMD   : %4lld ms  (result prefix: 0x%08llx)\n",
        simdMs,
        static_cast<unsigned long long>(sink & 0xFFFFFFFFull));

    if (simdMs > 0 && scalarMs > 0)
        std::printf("  Speedup: %.2fx\n",
            static_cast<double>(scalarMs) / static_cast<double>(simdMs));
    else
        std::printf("  Speedup: (time too small to measure reliably)\n");
#else
    std::printf("  [INFO] SIMD_ENABLED not defined – SIMD benchmark skipped\n");
#endif
}

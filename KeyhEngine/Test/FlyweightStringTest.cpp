#include "TestCommon.h"
#include "FlyweightStringTest.h"
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cstdio>
#include "FlyweightString.h"
#include "StaticString.h"
#include "StringView.h"

using namespace keyh;

// Convenience type aliases
using FW1A  = FlyweightString<char>;
using FW1W  = FlyweightString<wchar_t>;
using FW2A  = FlyweightString<char,    FNV1aHash<StringView<char>>,    StringPool2<char>>;
using FW2W  = FlyweightString<wchar_t, FNV1aHash<StringView<wchar_t>>, StringPool2<wchar_t>>;

// ---------------------------------------------------------------------------
// Construction correctness
// ---------------------------------------------------------------------------

void test_FlyweightString_construction()
{
    printSection("FlyweightString - construction");

    // From const char*
    FW1A fromRaw("engine");
    CHECK(!fromRaw.empty());
    CHECK(fromRaw.length() == 6);
    CHECK(fromRaw.size() == 6);
    CHECK(std::strcmp(fromRaw.c_str(), "engine") == 0);
    CHECK(fromRaw.getHash() != HashUtil::kInvalidHash);

    // From StaticString<char>
    StaticString<char> staticStr("keyh");
    FW1A fromStatic(staticStr);
    CHECK(!fromStatic.empty());
    CHECK(fromStatic.length() == 4);
    CHECK(std::strcmp(fromStatic.c_str(), "keyh") == 0);

    // From StringView<char>
    StringView<char> sv("world", 5);
    FW1A fromView(sv);
    CHECK(!fromView.empty());
    CHECK(fromView.length() == 5);
    CHECK(std::strcmp(fromView.c_str(), "world") == 0);

    // Empty-constructed FlyweightString has no valid hash yet
    FW1A defaultConstructed;
    CHECK(defaultConstructed.empty());
    CHECK(defaultConstructed.length() == 0);
    CHECK(defaultConstructed.getHash() == HashUtil::kInvalidHash);
}

// ---------------------------------------------------------------------------
// Interning (flyweight) invariant
// ---------------------------------------------------------------------------

void test_FlyweightString_interning()
{
    printSection("FlyweightString - interning (StringPool1)");

    // Two FlyweightStrings built from the same string must share the same
    // underlying char pointer (the pool interns the string exactly once).
    FW1A a("interned");
    FW1A b("interned");

    CHECK(a.c_str() == b.c_str());      // same pointer
    CHECK(a.getHash() == b.getHash());  // same hash

    // Different strings must not share a pointer
    FW1A c("other");
    CHECK(a.c_str() != c.c_str());
    CHECK(a.getHash() != c.getHash());

    // Long string (beyond SSO) is also interned correctly
    FW1A long1("this string is longer than the sso buffer indeed yes");
    FW1A long2("this string is longer than the sso buffer indeed yes");
    CHECK(long1.c_str() == long2.c_str());
    CHECK(long1.getHash() == long2.getHash());
}

// ---------------------------------------------------------------------------
// wchar_t variant
// ---------------------------------------------------------------------------

void test_FlyweightString_wchar()
{
    printSection("FlyweightString - wchar_t (StringPool1)");

    FW1W wa(L"engine");
    FW1W wb(L"engine");
    CHECK(wa.c_str() == wb.c_str());
    CHECK(wa.getHash() == wb.getHash());
    CHECK(std::wcscmp(wa.c_str(), L"engine") == 0);

    FW1W wc(L"other");
    CHECK(wa.c_str() != wc.c_str());

    // From StaticString<wchar_t>
    StaticString<wchar_t> ws(L"hello");
    FW1W fromStaticW(ws);
    CHECK(std::wcscmp(fromStaticW.c_str(), L"hello") == 0);

    // From StringView<wchar_t>
    StringView<wchar_t> svw(L"wide", 4);
    FW1W fromViewW(svw);
    CHECK(std::wcscmp(fromViewW.c_str(), L"wide") == 0);
}

// ---------------------------------------------------------------------------
// Getter API
// ---------------------------------------------------------------------------

void test_FlyweightString_getters()
{
    printSection("FlyweightString - getters");

    FW1A fw("getters");
    CHECK(fw.length() == 7);
    CHECK(fw.size() == 7);
    CHECK(!fw.empty());
    CHECK(fw.c_str() != nullptr);
    CHECK(std::strcmp(fw.getStringView().c_str(), "getters") == 0);
    CHECK(fw.getStringView().length() == 7);
    CHECK(fw.getHash() != HashUtil::kInvalidHash);
}

// ---------------------------------------------------------------------------
// StringPool1 vs StringPool2 reference check
// ---------------------------------------------------------------------------
// Both pools must satisfy the flyweight invariant: inserting the same string
// twice returns a StringView whose c_str() pointer is identical.
//
// StringPool1 stores each unique string as a StaticString inside a HashSet.
//   - Allocation: one heap allocation (or SSO) per unique string.
//   - Lookup overhead: HashSet traversal, then strcmp comparison.
//
// StringPool2 stores all unique strings in a single flat StaticArray buffer
// and tracks their positions in a HashMap<StringView, StringOffset>.
//   - Allocation: zero additional heap allocations (buffer is pre-allocated).
//   - Lookup overhead: HashMap traversal, then strcmp comparison.
//   - More cache-friendly because all strings are contiguous in memory.
//
// The pointer equality checks below (c_str() == c_str()) confirm that both
// pools correctly intern each string and hand out the same pointer on
// subsequent requests for the same content.
// ---------------------------------------------------------------------------

void test_StringPool_reference_check()
{
    printSection("StringPool1 vs StringPool2 - reference check");

    const char* testStrings[] = {
        "alpha", "beta", "gamma", "delta", "epsilon",
        "alpha", "beta", "gamma", "delta", "epsilon"   // duplicates
    };
    constexpr int kCount = 5;  // unique count

    // --- StringPool1 ---
    FW1A pool1[kCount * 2];
    for (int i = 0; i < kCount * 2; ++i)
        pool1[i] = FW1A(testStrings[i]);

    std::printf("\n  [StringPool1] pointer equality for duplicate inserts:\n");
    bool pool1Ok = true;
    for (int i = 0; i < kCount; ++i)
    {
        bool same = (pool1[i].c_str() == pool1[i + kCount].c_str());
        std::printf("    \"%s\": first=%p  second=%p  same=%s\n",
            testStrings[i],
            static_cast<const void*>(pool1[i].c_str()),
            static_cast<const void*>(pool1[i + kCount].c_str()),
            same ? "YES" : "NO");
        if (!same) pool1Ok = false;
    }
    CHECK(pool1Ok);

    // --- StringPool2 ---
    FW2A pool2[kCount * 2];
    for (int i = 0; i < kCount * 2; ++i)
        pool2[i] = FW2A(testStrings[i]);

    std::printf("\n  [StringPool2] pointer equality for duplicate inserts:\n");
    bool pool2Ok = true;
    for (int i = 0; i < kCount; ++i)
    {
        bool same = (pool2[i].c_str() == pool2[i + kCount].c_str());
        std::printf("    \"%s\": first=%p  second=%p  same=%s\n",
            testStrings[i],
            static_cast<const void*>(pool2[i].c_str()),
            static_cast<const void*>(pool2[i + kCount].c_str()),
            same ? "YES" : "NO");
        if (!same) pool2Ok = false;
    }
    CHECK(pool2Ok);

    // Pool1 and Pool2 intern the same string into different backing stores,
    // so their pointers differ even for identical content.
    CHECK(pool1[0].c_str() != pool2[0].c_str());

    std::printf("\n  Summary:\n");
    std::printf("    StringPool1: per-string StaticString nodes in a HashSet.\n");
    std::printf("                 Each unique string may allocate separately.\n");
    std::printf("    StringPool2: flat contiguous buffer + offset HashMap.\n");
    std::printf("                 No extra heap allocations after pool creation.\n");
    std::printf("                 Better cache locality for sequential lookups.\n");
}

void test_FlyweightString_performance()
{
    printSection("FlyweightString - performance (time)");

    constexpr const char* kWords[] = {
        "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta"
    };
    constexpr int kWordCount = static_cast<int>(sizeof(kWords) / sizeof(kWords[0]));
    const int kDefaultIterations = 200000;
    int iterations = kDefaultIterations;
    if (const char* envIter = std::getenv("KEYH_FLYWEIGHT_BENCH_ITERS"))
    {
        const int parsed = std::atoi(envIter);
        if (parsed > 0)
        {
            iterations = parsed;
        }
    }

    auto benchmarkPool1 = [&]() -> double
    {
        volatile size_t sink = 0;
        const auto begin = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i)
        {
            FW1A s(kWords[i % kWordCount]);
            sink ^= s.getHash();
        }
        const auto end = std::chrono::high_resolution_clock::now();
        (void)sink;
        return std::chrono::duration<double, std::milli>(end - begin).count();
    };

    auto benchmarkPool2 = [&]() -> double
    {
        volatile size_t sink = 0;
        const auto begin = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i)
        {
            FW2A s(kWords[i % kWordCount]);
            sink ^= s.getHash();
        }
        const auto end = std::chrono::high_resolution_clock::now();
        (void)sink;
        return std::chrono::duration<double, std::milli>(end - begin).count();
    };

    const double pool1Ms = benchmarkPool1();
    const double pool2Ms = benchmarkPool2();

    std::printf("  StringPool1: %.3f ms (%d iterations)\n", pool1Ms, iterations);
    std::printf("  StringPool2: %.3f ms (%d iterations)\n", pool2Ms, iterations);
    if (pool2Ms > 0.0)
    {
        std::printf("  ratio(pool1/pool2): %.3f\n", pool1Ms / pool2Ms);
    }

    CHECK(pool1Ms > 0.0);
    CHECK(pool2Ms > 0.0);
}

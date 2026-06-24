#include <cstdio>
#include <cstring>
#include <cwchar>
#include <thread>
#include <vector>
#include <atomic>
#include "StaticString.h"
#include "Ptr.h"
#include "RefPtr.h"
#include "WeakPtr.h"
#include "FlyweightString.h"

using namespace keyh;

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(expr) \
    do { \
        if (expr) { \
            ++g_pass; \
            std::printf("  [PASS] %s\n", #expr); \
        } else { \
            ++g_fail; \
            std::printf("  [FAIL] %s  (line %d)\n", #expr, __LINE__); \
        } \
    } while (0)

static void printSection(const char* name)
{
    std::printf("\n=== %s ===\n", name);
}

static void printSummary()
{
    std::printf("\n==============================\n");
    std::printf("Results: %d passed, %d failed\n", g_pass, g_fail);
    std::printf("==============================\n");
}

static constexpr char kShortText[] = "hello";
static constexpr char kLongText[] = "this string is longer than the sso buffer";
static constexpr wchar_t kShortWideText[] = L"world";
static constexpr wchar_t kLongWideText[] = L"this wide string is longer than the sso buffer";

static_assert((sizeof(kShortText) - 1) < StrUtil::ssoCapacity);
static_assert((sizeof(kLongText) - 1) >= StrUtil::ssoCapacity);
static_assert((sizeof(kShortWideText) / sizeof(wchar_t) - 1) < StrUtil::ssoCapacity);
static_assert((sizeof(kLongWideText) / sizeof(wchar_t) - 1) >= StrUtil::ssoCapacity);

static void test_StaticString_create_and_length()
{
    printSection("StaticString - create and length");

    StaticString<char> empty;
    CHECK(empty.empty());
    CHECK(empty.length() == 0);
    CHECK(empty.size() == 0);

    StaticString<char> shortString(kShortText);
    CHECK(!shortString.empty());
    CHECK(shortString.length() == 5);
    CHECK(shortString.size() == 5);
    CHECK(std::strcmp(shortString.c_str(), kShortText) == 0);

    StaticString<char> longString(kLongText);
    CHECK(!longString.empty());
    CHECK(longString.length() == std::strlen(kLongText));
    CHECK(longString.size() == std::strlen(kLongText));
    CHECK(std::strcmp(longString.c_str(), kLongText) == 0);
}

static void test_StaticString_copy()
{
    printSection("StaticString - copy");

    StaticString<char> shortSource(kShortText);
    StaticString<char> shortCopy(shortSource);
    CHECK(shortCopy == shortSource);
    shortCopy[0] = 'H';
    CHECK(shortSource[0] == 'h');
    CHECK(shortCopy[0] == 'H');
    CHECK(std::strcmp(shortCopy.c_str(), "Hello") == 0);
    CHECK(shortCopy != shortSource);

    StaticString<char> longSource(kLongText);
    StaticString<char> longCopy("seed");
    longCopy = longSource;
    CHECK(longCopy == longSource);
    longCopy[5] = '_';
    CHECK(longSource[5] != '_');
    CHECK(longCopy[5] == '_');
}

static void test_StaticString_move()
{
    printSection("StaticString - move");

    StaticString<char> shortSource(kShortText);
    StaticString<char> shortMoved(keyh::move(shortSource));
    CHECK(std::strcmp(shortMoved.c_str(), kShortText) == 0);
    CHECK(shortMoved.length() == std::strlen(kShortText));
    CHECK(shortSource.empty());
    CHECK(shortSource.length() == 0);

    StaticString<char> longSource(kLongText);
    StaticString<char> longMoved("temporary");
    longMoved = keyh::move(longSource);
    CHECK(std::strcmp(longMoved.c_str(), kLongText) == 0);
    CHECK(longMoved.length() == std::strlen(kLongText));
    CHECK(longSource.empty());
    CHECK(longSource.length() == 0);
}

static void test_StaticString_clear_and_reuse()
{
    printSection("StaticString - clear and reuse");

    StaticString<char> value(kLongText);
    value.clear();
    CHECK(value.empty());
    CHECK(value.length() == 0);
    CHECK(value.size() == 0);

    value = StaticString<char>("abc");
    CHECK(!value.empty());
    CHECK(value.length() == 3);
    CHECK(std::strcmp(value.c_str(), "abc") == 0);
}

static void test_StaticString_index_access()
{
    printSection("StaticString - index access");

    StaticString<char> shortString(kShortText);
    CHECK(shortString[0] == 'h');
    CHECK(shortString[4] == 'o');
    shortString[1] = 'u';
    CHECK(shortString[1] == 'u');
    CHECK(std::strcmp(shortString.c_str(), "hullo") == 0);

    const StaticString<char> longString(kLongText);
    CHECK(longString[0] == 't');
    CHECK(longString[longString.length() - 1] == 'r');
}

static void test_StaticString_wchar()
{
    printSection("StaticString - wchar_t");

    StaticString<wchar_t> shortString(kShortWideText);
    CHECK(shortString.length() == 5);
    CHECK(std::wcscmp(shortString.c_str(), kShortWideText) == 0);

    StaticString<wchar_t> longString(kLongWideText);
    CHECK(longString.length() == std::wcslen(kLongWideText));
    CHECK(std::wcscmp(longString.c_str(), kLongWideText) == 0);

    StaticString<wchar_t> copied(longString);
    CHECK(copied == longString);
    copied[0] = L'T';
    CHECK(longString[0] == L't');
    CHECK(copied[0] == L'T');
}

// -----------------------------------------------------------------------
// Helpers for RefPtr / WeakPtr tests
// -----------------------------------------------------------------------

struct TrackedObj
{
    int value;
    int* destroyCount;
    TrackedObj(int v, int* dc) : value(v), destroyCount(dc) {}
    ~TrackedObj() { if (destroyCount) ++(*destroyCount); }
};

struct Base
{
    int x;
    int* destroyCount;
    Base(int v, int* dc) : x(v), destroyCount(dc) {}
    virtual ~Base() { if (destroyCount) ++(*destroyCount); }
};

struct Derived : public Base
{
    int y;
    Derived(int v, int extra, int* dc) : Base(v, dc), y(extra) {}
};

// -----------------------------------------------------------------------
// RefPtr tests
// -----------------------------------------------------------------------

static void test_RefPtr_basic()
{
    printSection("RefPtr - basic");

    RefPtr<int> p = makeRefPtr<int>(42);
    CHECK(p != nullptr);
    CHECK(*p.get() == 42);

    p = nullptr;
    CHECK(p == nullptr);
    CHECK(p.get() == nullptr);
}

static void test_RefPtr_copy()
{
    printSection("RefPtr - copy");

    int dc = 0;
    {
        RefPtr<TrackedObj> a = makeRefPtr<TrackedObj>(1, &dc);
        CHECK(a != nullptr);
        CHECK(a->value == 1);

        RefPtr<TrackedObj> b = a;
        CHECK(b != nullptr);
        CHECK(b->value == 1);
        CHECK(a == b);
        CHECK(dc == 0);    // still alive

        a = nullptr;
        CHECK(a == nullptr);
        CHECK(dc == 0);    // b still holds reference

        b = nullptr;
        CHECK(dc == 1);    // last ref released, object destroyed
    }
    CHECK(dc == 1);        // no double-free
}

static void test_RefPtr_copy_assign()
{
    printSection("RefPtr - copy assign");

    int dc = 0;
    RefPtr<TrackedObj> a = makeRefPtr<TrackedObj>(10, &dc);
    RefPtr<TrackedObj> b = makeRefPtr<TrackedObj>(20, &dc);

    b = a;
    CHECK(dc == 1);        // original b object destroyed
    CHECK(b->value == 10);

    // self-assign must not corrupt
    a = a;
    CHECK(a != nullptr);
    CHECK(a->value == 10);
    CHECK(dc == 1);

    a = nullptr;
    CHECK(dc == 1);        // b still holds reference
    b = nullptr;
    CHECK(dc == 2);        // last shared object destroyed
}

static void test_RefPtr_polymorphism()
{
    printSection("RefPtr - polymorphism");

    int dc = 0;
    {
        RefPtr<Base> p = makeRefPtr<Base, Derived>(5, 7, &dc);
        CHECK(p != nullptr);
        CHECK(p->x == 5);
    }
    CHECK(dc == 1);

    dc = 0;
    {
        RefPtr<Derived> d = makeRefPtr<Derived>(3, 9, &dc);
        RefPtr<Base> b = d;          // copy via cross-type ctor
        CHECK(b->x == 3);
        CHECK(d->y == 9);
        d = nullptr;
        CHECK(dc == 0);              // b still alive
    }
    CHECK(dc == 1);
}

// -----------------------------------------------------------------------
// WeakPtr tests
// -----------------------------------------------------------------------

static void test_WeakPtr_basic()
{
    printSection("WeakPtr - basic");

    RefPtr<int> r = makeRefPtr<int>(99);
    WeakPtr<int> w = r;

    CHECK(w != nullptr);
    CHECK(w.isValid());
    CHECK(*w.get() == 99);
}

static void test_WeakPtr_lifetime()
{
    printSection("WeakPtr - lifetime");

    WeakPtr<int> w;
    CHECK(w == nullptr);
    CHECK(!w.isValid());

    {
        RefPtr<int> r = makeRefPtr<int>(7);
        w = r;
        CHECK(w.isValid());
        CHECK(*w.get() == 7);
    }
    // r is destroyed, object is gone
    CHECK(!w.isValid());
}

static void test_WeakPtr_copy()
{
    printSection("WeakPtr - copy");

    RefPtr<int> r = makeRefPtr<int>(55);
    WeakPtr<int> w1 = r;
    WeakPtr<int> w2 = w1;          // WeakPtr copy ctor

    CHECK(w1.isValid());
    CHECK(w2.isValid());
    CHECK(w1 == w2);

    r = nullptr;
    CHECK(!w1.isValid());
    CHECK(!w2.isValid());
}

static void test_WeakPtr_assign_null()
{
    printSection("WeakPtr - assign null");

    int dc = 0;
    RefPtr<TrackedObj> r = makeRefPtr<TrackedObj>(3, &dc);
    WeakPtr<TrackedObj> w = r;
    CHECK(w.isValid());

    w = nullptr;
    CHECK(!w.isValid());
    CHECK(w == nullptr);
    CHECK(dc == 0);    // object still alive via r

    r = nullptr;
    CHECK(dc == 1);
}

static void test_WeakPtr_reassign()
{
    printSection("WeakPtr - reassign");

    RefPtr<int> r1 = makeRefPtr<int>(1);
    RefPtr<int> r2 = makeRefPtr<int>(2);
    WeakPtr<int> w = r1;

    CHECK(*w.get() == 1);
    w = r2;
    CHECK(*w.get() == 2);

    r2 = nullptr;
    CHECK(!w.isValid());
}

static void test_WeakPtr_control_block_freed()
{
    printSection("WeakPtr - control block freed after all refs gone");

    // Verify no crash or leak: WeakPtr outlives RefPtr
    WeakPtr<int> w;
    {
        RefPtr<int> r = makeRefPtr<int>(42);
        w = r;
        CHECK(w.isValid());
    }
    CHECK(!w.isValid());
    // WeakPtr destructor releases its weak ref; no crash
}

// -----------------------------------------------------------------------
// Concurrency tests
// -----------------------------------------------------------------------

static void test_RefPtr_concurrency()
{
    printSection("RefPtr - concurrency");

    int dc = 0;
    {
        RefPtr<TrackedObj> shared = makeRefPtr<TrackedObj>(0, &dc);
        constexpr int kThreads = 8;
        constexpr int kCopiesPerThread = 1000;
        std::vector<std::thread> threads;
        threads.reserve(kThreads);
        for (int i = 0; i < kThreads; ++i)
        {
            threads.emplace_back([shared]()
            {
                for (int j = 0; j < kCopiesPerThread; ++j)
                {
                    RefPtr<TrackedObj> local = shared;
                    volatile int v = local->value;
                    (void)v;
                }
            });
        }
        for (auto& t : threads) t.join();
        CHECK(dc == 0);    // object still alive
    }
    CHECK(dc == 1);        // destroyed exactly once after all threads done
}

static void test_WeakPtr_concurrency()
{
    printSection("WeakPtr - concurrency");

    RefPtr<int> shared = makeRefPtr<int>(77);
    constexpr int kThreads = 8;
    std::atomic<int> validCount{ 0 };
    std::vector<std::thread> threads;
    threads.reserve(kThreads);
    for (int i = 0; i < kThreads; ++i)
    {
        threads.emplace_back([&shared, &validCount]()
        {
            WeakPtr<int> w = shared;
            if (w.isValid()) validCount.fetch_add(1, std::memory_order_relaxed);
        });
    }
    for (auto& t : threads) t.join();
    CHECK(validCount.load() == kThreads);
}

static void test_RefPtr_WeakPtr_concurrency_mixed()
{
    printSection("RefPtr/WeakPtr - concurrent copy/release and weak check");

    int dc = 0;
    RefPtr<TrackedObj> root = makeRefPtr<TrackedObj>(42, &dc);
    constexpr int kThreads = 4;
    constexpr int kIter = 500;
    std::atomic<int> validCount{ 0 };
    std::vector<std::thread> threads;
    threads.reserve(kThreads * 2);

    // Half threads copy/release RefPtr; half create WeakPtrs and check validity
    for (int i = 0; i < kThreads; ++i)
    {
        threads.emplace_back([root]()
        {
            for (int j = 0; j < kIter; ++j)
            {
                RefPtr<TrackedObj> local = root;
                volatile int v = local->value;
                (void)v;
            }
        });
        threads.emplace_back([root, &validCount]()
        {
            // root is captured by value so object is always alive here
            for (int j = 0; j < kIter; ++j)
            {
                WeakPtr<TrackedObj> w = root;
                if (w.isValid()) validCount.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& t : threads) t.join();
    CHECK(dc == 0);
    CHECK(validCount.load() == kThreads * kIter);
}

int main()
{
    test_StaticString_create_and_length();
    test_StaticString_copy();
    test_StaticString_move();
    test_StaticString_clear_and_reuse();
    test_StaticString_index_access();
    test_StaticString_wchar();

    test_RefPtr_basic();
    test_RefPtr_copy();
    test_RefPtr_copy_assign();
    test_RefPtr_polymorphism();

    test_WeakPtr_basic();
    test_WeakPtr_lifetime();
    test_WeakPtr_copy();
    test_WeakPtr_assign_null();
    test_WeakPtr_reassign();
    test_WeakPtr_control_block_freed();

    test_RefPtr_concurrency();
    test_WeakPtr_concurrency();
    test_RefPtr_WeakPtr_concurrency_mixed();

    printSummary();

	Ptr<int> t = makePtr<int>(42);

	FlyweightString<char> fw1("hello");

    return g_fail == 0 ? 0 : 1;
}

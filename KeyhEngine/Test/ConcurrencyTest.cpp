#include "TestCommon.h"
#include "ConcurrencyTest.h"
#include "RefPtrTest.h"
#include "RefPtr.h"
#include "WeakPtr.h"
#include <thread>
#include <vector>
#include <atomic>

using namespace keyh;

void test_RefPtr_concurrency()
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

void test_WeakPtr_concurrency()
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

void test_RefPtr_WeakPtr_concurrency_mixed()
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

#include "TestCommon.h"
#include "WeakPtrTest.h"
#include "RefPtrTest.h"
#include "RefPtr.h"
#include "WeakPtr.h"

using namespace keyh;

void test_WeakPtr_basic()
{
    printSection("WeakPtr - basic");

    RefPtr<int> r = makeRefPtr<int>(99);
    WeakPtr<int> w = r;

    CHECK(w != nullptr);
    CHECK(w.isValid());
    CHECK(*w.get() == 99);
}

void test_WeakPtr_lifetime()
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

void test_WeakPtr_copy()
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

void test_WeakPtr_assign_null()
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

void test_WeakPtr_reassign()
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

void test_WeakPtr_control_block_freed()
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

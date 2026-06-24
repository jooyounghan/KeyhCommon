#include "TestCommon.h"
#include "RefPtrTest.h"
#include "RefPtr.h"

using namespace keyh;

void test_RefPtr_basic()
{
    printSection("RefPtr - basic");

    RefPtr<int> p = makeRefPtr<int>(42);
    CHECK(p != nullptr);
    CHECK(*p.get() == 42);

    p = nullptr;
    CHECK(p == nullptr);
    CHECK(p.get() == nullptr);
}

void test_RefPtr_copy()
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

void test_RefPtr_copy_assign()
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

void test_RefPtr_polymorphism()
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

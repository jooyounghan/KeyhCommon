#pragma once

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

void test_RefPtr_basic();
void test_RefPtr_copy();
void test_RefPtr_copy_assign();
void test_RefPtr_polymorphism();

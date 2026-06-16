#define NOMINMAX

#include <iostream>
#include <string>
#include <Windows.h>
#include "HashMap.h"
#include "HashSet.h"
using namespace keyh;

// ─────────────────────────────────────────────
// Simple test harness
// ─────────────────────────────────────────────
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(expr) \
    do { \
        if (expr) { \
            ++g_pass; \
            printf("  [PASS] %s\n", #expr); \
        } else { \
            ++g_fail; \
            printf("  [FAIL] %s  (line %d)\n", #expr, __LINE__); \
        } \
    } while (0)

static void printSection(const char* name)
{
    printf("\n=== %s ===\n", name);
}

static void printSummary()
{
    printf("\n==============================\n");
    printf("Results: %d passed, %d failed\n", g_pass, g_fail);
    printf("==============================\n");
}

// ─────────────────────────────────────────────
// Custom Hashers for collision / edge-case tests
// ─────────────────────────────────────────────

// Always returns 0 → every key maps to slot 0 (maximum collision)
struct AlwaysZeroHasher
{
    size_t operator()(int) const noexcept { return 0; }
};

// Returns key % 4 → keys 0,4,8,… all map to the same initial slot;
// creates controlled cross-slot Robin Hood displacement.
struct Mod4Hasher
{
    size_t operator()(int key) const noexcept
    {
        return static_cast<size_t>(key % 4);
    }
};

// ─────────────────────────────────────────────
// Test functions
// ─────────────────────────────────────────────

// ── HashMap: basic insert / find / remove ─────────────────────────────────
static void test_HashMap_basic()
{
    printSection("HashMap – basic insert / find / remove");

    HashMap<int, std::string> map;

    // Insert and find
    auto r1 = map.insert(1, "one");
    CHECK(r1.isSuccess());
    CHECK(r1._value == "one");

    auto r2 = map.insert(2, "two");
    CHECK(r2.isSuccess());

    auto f1 = map.find(1);
    CHECK(f1.isFound());
    CHECK(*f1.value() == "one");

    auto f99 = map.find(99);
    CHECK(!f99.isFound());
    CHECK(f99.value() == nullptr);

    // Duplicate insert without replace → denied
    auto rDup = map.insert(1, "ONE");
    CHECK(rDup.isDenied());
    CHECK(*map.find(1).value() == "one");   // value unchanged

    // Duplicate insert with replace → success, value updated
    auto rRepl = map.insert(1, "ONE", /*replace=*/true);
    CHECK(rRepl.isSuccess());
    CHECK(*map.find(1).value() == "ONE");

    // Remove existing key
    CHECK(map.remove(1) == true);
    CHECK(!map.find(1).isFound());

    // Remove non-existing key
    CHECK(map.remove(1) == false);
    CHECK(map.remove(99) == false);
}

// ── HashMap: auto-rehash ──────────────────────────────────────────────────
// kInitialCapacity=8, kMaxLoadFactor=0.75 → rehash triggers when
// size reaches 6 (0.75*8=6).  Inserting 10 items exercises at least
// one rehash; all items must still be findable afterwards.
static void test_HashMap_rehash()
{
    printSection("HashMap – auto-rehash (load-factor triggered)");

    HashMap<int, std::string> map;
    const char* vals[] = { "a","b","c","d","e","f","g","h","i","j" };
    for (int i = 0; i < 10; ++i)
        map.insert(i, vals[i]);

    for (int i = 0; i < 10; ++i)
    {
        auto r = map.find(i);
        CHECK(r.isFound());
        CHECK(*r.value() == vals[i]);
    }
}

// ── HashMap: reserve ──────────────────────────────────────────────────────
static void test_HashMap_reserve()
{
    printSection("HashMap – reserve then bulk insert");

    HashMap<int, std::string> map;
    map.reserve(64);

    for (int i = 0; i < 50; ++i)
        map.insert(i, "v");

    for (int i = 0; i < 50; ++i)
        CHECK(map.find(i).isFound());

    CHECK(!map.find(50).isFound());
}

// ── HashMap: move semantics ───────────────────────────────────────────────
static void test_HashMap_move()
{
    printSection("HashMap – move constructor / move assignment");

    HashMap<int, std::string> src;
    src.insert(10, "ten");
    src.insert(20, "twenty");

    // Move constructor
    HashMap<int, std::string> dst(keyh::move(src));
    CHECK(dst.find(10).isFound());
    CHECK(*dst.find(10).value() == "ten");
    CHECK(dst.find(20).isFound());

    // Moved-from map should be empty (no valid buckets)
    CHECK(!src.find(10).isFound());

    // Move assignment
    HashMap<int, std::string> dst2;
    dst2.insert(99, "ninety-nine");
    dst2 = keyh::move(dst);
    CHECK(dst2.find(10).isFound());
    CHECK(!dst2.find(99).isFound());  // old content gone
    CHECK(!dst.find(10).isFound());   // moved-from is empty
}

// ── HashMap: clear ────────────────────────────────────────────────────────
static void test_HashMap_clear()
{
    printSection("HashMap – clear");

    HashMap<int, std::string> map;
    for (int i = 0; i < 8; ++i)
        map.insert(i, "x");

    map.clear();

    for (int i = 0; i < 8; ++i)
        CHECK(!map.find(i).isFound());

    // After clear we can insert again
    auto r = map.insert(42, "hello");
    CHECK(r.isSuccess());
    CHECK(map.find(42).isFound());
}

// ── HashMap: Robin Hood collision – all keys same slot ────────────────────
static void test_HashMap_robinhood_allcollide()
{
    printSection("HashMap – Robin Hood: all keys collide (AlwaysZeroHasher)");

    HashMap<int, int, AlwaysZeroHasher> map;
    map.reserve(16);

    // Insert 6 keys – all hash to slot 0; Robin Hood lines them up linearly
    for (int i = 0; i < 6; ++i)
        map.insert(i, i * 10);

    // Every key must be found with correct value
    for (int i = 0; i < 6; ++i)
    {
        auto r = map.find(i);
        CHECK(r.isFound());
        CHECK(*r.value() == i * 10);
    }

    CHECK(!map.find(99).isFound());

    // Remove a key from the middle of the chain → backward shift
    CHECK(map.remove(2) == true);
    CHECK(!map.find(2).isFound());

    // Remaining keys must still be reachable
    CHECK(map.find(0).isFound());
    CHECK(map.find(1).isFound());
    CHECK(map.find(3).isFound());
    CHECK(map.find(4).isFound());
    CHECK(map.find(5).isFound());

    // Remove head of chain
    CHECK(map.remove(0) == true);
    CHECK(!map.find(0).isFound());
    CHECK(map.find(1).isFound());

    // Remove tail of chain
    CHECK(map.remove(5) == true);
    CHECK(!map.find(5).isFound());
    CHECK(map.find(4).isFound());

    // Re-insert after removal
    auto r = map.insert(2, 200);
    CHECK(r.isSuccess());
    CHECK(*map.find(2).value() == 200);
}

// ── HashMap: Robin Hood collision – cross-slot displacement ───────────────
// With Mod4Hasher and capacity=16:
//   key % 4 gives initial slot.  Keys 0,4,8 all start at slot 0;
//   keys 1,5,9 at slot 1; etc.
//   Insertions interleave and Robin Hood must displace richer entries.
static void test_HashMap_robinhood_mod4()
{
    printSection("HashMap – Robin Hood: cross-slot displacement (Mod4Hasher)");

    HashMap<int, int, Mod4Hasher> map;
    map.reserve(16);

    // Insert keys whose mod-4 values create displacement chains
    int keys[] = { 0, 1, 4, 5, 8, 9, 2, 6 };
    for (int k : keys)
        map.insert(k, k * 100);

    for (int k : keys)
    {
        auto r = map.find(k);
        CHECK(r.isFound());
        CHECK(*r.value() == k * 100);
    }

    // Remove and re-insert to verify backward-shift + re-insert
    CHECK(map.remove(4) == true);
    CHECK(!map.find(4).isFound());
    CHECK(map.find(0).isFound());
    CHECK(map.find(8).isFound());

    map.insert(4, 400);
    CHECK(*map.find(4).value() == 400);

    // Replace via re-insert with replace=true
    map.insert(0, 9999, /*replace=*/true);
    CHECK(*map.find(0).value() == 9999);
}

// ── HashSet: basic ────────────────────────────────────────────────────────
static void test_HashSet_basic()
{
    printSection("HashSet – basic insert / contains / remove");

    HashSet<int> set;

    auto r1 = set.insert(10);
    CHECK(r1.isSuccess());
    CHECK(set.contains(10));
    CHECK(!set.contains(99));

    // Duplicate insert → denied
    auto rDup = set.insert(10);
    CHECK(rDup.isDenied());

    // Remove
    CHECK(set.remove(10) == true);
    CHECK(!set.contains(10));
    CHECK(set.remove(10) == false);
    CHECK(set.remove(99) == false);
}

// ── HashSet: rehash & reserve ─────────────────────────────────────────────
static void test_HashSet_rehash_reserve()
{
    printSection("HashSet – rehash and reserve");

    HashSet<int> set;
    for (int i = 0; i < 20; ++i)
        set.insert(i);

    for (int i = 0; i < 20; ++i)
        CHECK(set.contains(i));

    HashSet<int> set2;
    set2.reserve(128);
    for (int i = 0; i < 100; ++i)
        set2.insert(i);

    for (int i = 0; i < 100; ++i)
        CHECK(set2.contains(i));
}

// ── HashSet: move semantics & clear ──────────────────────────────────────
static void test_HashSet_move_clear()
{
    printSection("HashSet – move semantics and clear");

    HashSet<int> src;
    src.insert(1);
    src.insert(2);

    HashSet<int> dst(keyh::move(src));
    CHECK(dst.contains(1));
    CHECK(dst.contains(2));
    CHECK(!src.contains(1));

    HashSet<int> dst2;
    dst2.insert(99);
    dst2 = keyh::move(dst);
    CHECK(dst2.contains(1));
    CHECK(!dst2.contains(99));

    dst2.clear();
    CHECK(!dst2.contains(1));
    dst2.insert(42);
    CHECK(dst2.contains(42));
}

// ── HashSet: Robin Hood collision ─────────────────────────────────────────
static void test_HashSet_robinhood_allcollide()
{
    printSection("HashSet – Robin Hood: all keys collide (AlwaysZeroHasher)");

    HashSet<int, AlwaysZeroHasher> set;
    set.reserve(16);

    for (int i = 0; i < 6; ++i)
        set.insert(i);

    for (int i = 0; i < 6; ++i)
        CHECK(set.contains(i));

    CHECK(!set.contains(99));

    // Remove middle element → backward shift
    CHECK(set.remove(3) == true);
    CHECK(!set.contains(3));
    CHECK(set.contains(0));
    CHECK(set.contains(1));
    CHECK(set.contains(2));
    CHECK(set.contains(4));
    CHECK(set.contains(5));

    // Remove head
    CHECK(set.remove(0) == true);
    CHECK(!set.contains(0));
    CHECK(set.contains(1));

    // Re-insert removed element
    CHECK(set.insert(3).isSuccess());
    CHECK(set.contains(3));
    // Duplicate now denied
    CHECK(set.insert(3).isDenied());
}

// ── HashSet: Robin Hood cross-slot displacement ───────────────────────────
static void test_HashSet_robinhood_mod4()
{
    printSection("HashSet – Robin Hood: cross-slot displacement (Mod4Hasher)");

    HashSet<int, Mod4Hasher> set;
    set.reserve(16);

    int keys[] = { 0, 4, 8, 12, 1, 5, 9, 2, 6 };
    for (int k : keys)
        set.insert(k);

    for (int k : keys)
        CHECK(set.contains(k));

    // Remove and verify chain integrity
    CHECK(set.remove(8) == true);
    CHECK(!set.contains(8));
    CHECK(set.contains(0));
    CHECK(set.contains(4));
    CHECK(set.contains(12));

    // Re-insert
    CHECK(set.insert(8).isSuccess());
    CHECK(set.contains(8));
}

// ── std::string key: HashMap ──────────────────────────────────────────────
static void test_HashMap_string_key()
{
    printSection("HashMap – std::string key (FNV1aHash<std::string> specialization)");

    HashMap<std::string, int> map;

    map.insert("apple",  1);
    map.insert("banana", 2);
    map.insert("cherry", 3);

    auto fa = map.find("apple");
    CHECK(fa.isFound());
    CHECK(*fa.value() == 1);

    auto fb = map.find("banana");
    CHECK(fb.isFound());
    CHECK(*fb.value() == 2);

    CHECK(!map.find("durian").isFound());

    // Replace
    map.insert("apple", 100, /*replace=*/true);
    CHECK(*map.find("apple").value() == 100);

    // Duplicate denied
    CHECK(map.insert("banana", 999).isDenied());
    CHECK(*map.find("banana").value() == 2);

    // Remove
    CHECK(map.remove("cherry") == true);
    CHECK(!map.find("cherry").isFound());
    CHECK(map.remove("cherry") == false);

    // Rehash with string keys
    for (int i = 0; i < 20; ++i)
        map.insert(std::to_string(i), i);

    for (int i = 0; i < 20; ++i)
    {
        auto r = map.find(std::to_string(i));
        CHECK(r.isFound());
        CHECK(*r.value() == i);
    }
}

// ── std::string key: HashSet ──────────────────────────────────────────────
static void test_HashSet_string_key()
{
    printSection("HashSet – std::string key (FNV1aHash<std::string> specialization)");

    HashSet<std::string> set;

    CHECK(set.insert("hello").isSuccess());
    CHECK(set.insert("world").isSuccess());
    CHECK(set.contains("hello"));
    CHECK(set.contains("world"));
    CHECK(!set.contains("foo"));

    CHECK(set.insert("hello").isDenied());

    CHECK(set.remove("hello") == true);
    CHECK(!set.contains("hello"));
    CHECK(set.remove("hello") == false);

    // Bulk insert / rehash with string keys
    for (int i = 0; i < 30; ++i)
        set.insert(std::to_string(i));

    for (int i = 0; i < 30; ++i)
        CHECK(set.contains(std::to_string(i)));
}

// ─────────────────────────────────────────────
// main
// ─────────────────────────────────────────────
int main()
{
    test_HashMap_basic();
    test_HashMap_rehash();
    test_HashMap_reserve();
    test_HashMap_move();
    test_HashMap_clear();
    test_HashMap_robinhood_allcollide();
    test_HashMap_robinhood_mod4();

    test_HashSet_basic();
    test_HashSet_rehash_reserve();
    test_HashSet_move_clear();
    test_HashSet_robinhood_allcollide();
    test_HashSet_robinhood_mod4();

    test_HashMap_string_key();
    test_HashSet_string_key();

    printSummary();
    return g_fail == 0 ? 0 : 1;
}

#include "TestCommon.h"
#include "StaticStringTest.h"
#include "RefPtrTest.h"
#include "WeakPtrTest.h"
#include "ConcurrencyTest.h"
#include "FlyweightStringTest.h"

int g_pass = 0;
int g_fail = 0;

int main()
{
    //test_StaticString_create_and_length();
    //test_StaticString_copy();
    //test_StaticString_move();
    //test_StaticString_clear_and_reuse();
    //test_StaticString_index_access();
    //test_StaticString_wchar();

    //test_RefPtr_basic();
    //test_RefPtr_copy();
    //test_RefPtr_copy_assign();
    //test_RefPtr_polymorphism();

    //test_WeakPtr_basic();
    //test_WeakPtr_lifetime();
    //test_WeakPtr_copy();
    //test_WeakPtr_assign_null();
    //test_WeakPtr_reassign();
    //test_WeakPtr_control_block_freed();

    //test_RefPtr_concurrency();
    //test_WeakPtr_concurrency();
    //test_RefPtr_WeakPtr_concurrency_mixed();

    test_FlyweightString_construction();
    test_FlyweightString_interning();
    test_FlyweightString_wchar();
    test_FlyweightString_getters();
    test_StringPool_reference_check();
    test_FlyweightString_performance();

    printSummary();

    return g_fail == 0 ? 0 : 1;
}

#include "test.h"

#include "StaticStringTest.h"
#include "RefPtrTest.h"
#include "WeakPtrTest.h"
#include "ConcurrencyTest.h"
#include "FlyweightStringTest.h"
#include "BufferTest.h"
#include "JsonTest.h"
int g_pass = 0;
int g_fail = 0;

using namespace keyh;



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

    //test_StaticBuffer_basic();
    //test_DynamicBuffer_allocate_and_grow();

    test_Json_parse_test_file();
    test_Json_traverse_root_object();
    test_Json_traverse_array();
    test_Json_traverse_nested_value();
    test_Json_traverse_empty_containers();
    test_Json_float_value_access();
    benchmark_Json_parse_speed();
    benchmark_Json_traversal_speed();

    printSummary();

    return g_fail == 0 ? 0 : 1;
}

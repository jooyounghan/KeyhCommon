#pragma once

void test_Reflect_serialize_primitives();
void test_Reflect_deserialize_primitives();
void test_Reflect_roundtrip_primitives();
void test_Reflect_roundtrip_vector();
void test_Reflect_roundtrip_nested_object();
void test_Reflect_large_object_file_roundtrip_compare();
void test_Reflect_enum_string_roundtrip();
void benchmark_Reflect_deserialize_512kb_x100();

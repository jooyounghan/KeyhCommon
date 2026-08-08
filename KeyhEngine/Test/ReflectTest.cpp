#include "TestCommon.h"
#include "ReflectTest.h"
#include "test.h"
#include "ReflectSerializer.h"
#include "DynamicBuffer.h"
#include "JsonDocument.h"
#include "File.h"

#include <chrono>
#include <cstring>
#include <cstdio>

using namespace keyh;

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────
namespace
{
    // Serialize a ReflectObject to a DynamicBufferA (in-memory JSON).
    class ReflectBufferProxy : public DynamicBuffer<char>
    {
    public:
        ~ReflectBufferProxy() override { this->resetImpl(); }
    };

    // Deserialize from an in-memory JSON string into a ReflectObject.
    static bool deserializeFromString(const char* jsonStr, size_t jsonSize, IReflectObject* obj)
    {
        JsonDocument doc;
        if (!doc.buildFromJsonString(jsonStr, jsonSize))
            return false;
        JsonObject root = doc.getRootObject();
        if (!root.isValid())
            return false;
        ReflectSerializer::deserializeObjectFromJson(root, obj);
        return true;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_serialize_primitives
//   Verifies that serializing a TestObject with known primitive values
//   produces a JSON string that contains the expected tokens.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_serialize_primitives()
{
    printSection("Reflect - serialize primitives to JSON");

    TestObject obj;
    obj._intValue    = 7;
    obj._floatValue  = 2.5f;
    obj._stringValue = StaticStringA("hello");

    ReflectBufferProxy buffer;
    buffer.allocate(1024);
    ReflectSerializer::serializeObjectToBuffer(&buffer, &obj);

    const char* data = buffer.getBuffer();
    const size_t size = buffer.size();

    CHECK(size > 0);

    // The serialized JSON must contain the integer 7
    bool found7 = false;
    for (size_t i = 0; i + 1 <= size; ++i)
    {
        if (data[i] == '7')
        {
            found7 = true;
            break;
        }
    }
    CHECK(found7);

    // Must contain "hello"
    const char* needle = "hello";
    const size_t needleLen = std::strlen(needle);
    bool foundHello = false;
    for (size_t i = 0; i + needleLen <= size; ++i)
    {
        if (std::memcmp(data + i, needle, needleLen) == 0)
        {
            foundHello = true;
            break;
        }
    }
    CHECK(foundHello);

    // Must start with '{'
    CHECK(size > 0 && data[0] == '{');
    // Must end with '}'
    CHECK(size > 0 && data[size - 1] == '}');

    std::printf("  serialized %zu bytes\n", size);
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_deserialize_primitives
//   Verifies that a hand-crafted JSON string is correctly deserialized into
//   the matching fields of a TestObject.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_deserialize_primitives()
{
    printSection("Reflect - deserialize primitives from JSON");

    const char* jsonStr =
        "{"
        "\"_intValue\":99,"
        "\"FloatValue\":1.5,"
        "\"_stringValue\":\"world\""
        "}";

    TestObject obj;
    const bool ok = deserializeFromString(jsonStr, std::strlen(jsonStr), &obj);
    CHECK(ok);

    CHECK(obj._intValue   == 99);

    const float diff = obj._floatValue - 1.5f;
    CHECK(diff > -0.001f && diff < 0.001f);

    CHECK(std::strcmp(obj._stringValue.c_str(), "world") == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_roundtrip_primitives
//   Serializes a TestObject with primitive values and deserializes it back,
//   checking that all field values are preserved.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_roundtrip_primitives()
{
    printSection("Reflect - round-trip primitives (serialize → deserialize)");

    TestObject src;
    src._intValue    = -123;
    src._floatValue  = 9.875f;
    src._stringValue = StaticStringA("roundtrip");

    // Serialize
    ReflectBufferProxy buffer;
    buffer.allocate(2048);
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src);

    // Deserialize
    TestObject dst;
    const bool ok = deserializeFromString(buffer.getBuffer(), buffer.size(), &dst);
    CHECK(ok);

    CHECK(dst._intValue == -123);

    const float diff = dst._floatValue - 9.875f;
    CHECK(diff > -0.001f && diff < 0.001f);

    CHECK(std::strcmp(dst._stringValue.c_str(), "roundtrip") == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_roundtrip_vector
//   Serializes a TestObject whose _names vector has several entries, then
//   deserializes and verifies the vector contents are preserved.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_roundtrip_vector()
{
    printSection("Reflect - round-trip Vector<StaticStringA>");

    TestObject src;
    src._names.push_back(StaticStringA("alpha"));
    src._names.push_back(StaticStringA("beta"));
    src._names.push_back(StaticStringA("gamma"));

    // Serialize
    ReflectBufferProxy buffer;
    buffer.allocate(2048);
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src);

    // Deserialize
    TestObject dst;
    const bool ok = deserializeFromString(buffer.getBuffer(), buffer.size(), &dst);
    CHECK(ok);

    CHECK(dst._names.size() == 3);
    if (dst._names.size() == 3)
    {
        CHECK(std::strcmp(dst._names[0].c_str(), "alpha") == 0);
        CHECK(std::strcmp(dst._names[1].c_str(), "beta")  == 0);
        CHECK(std::strcmp(dst._names[2].c_str(), "gamma") == 0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_roundtrip_nested_object
//   Serializes a TestObject that contains a nested TestSubObject, deserializes
//   it back, and checks that the nested object's fields are preserved.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_roundtrip_nested_object()
{
    printSection("Reflect - round-trip nested ReflectObject");

    TestObject src;
    src._intValue = 5;
    src._subObject._names.push_back(StaticStringA("sub_alpha"));
    src._subObject._names.push_back(StaticStringA("sub_beta"));

    // Serialize
    ReflectBufferProxy buffer;
    buffer.allocate(4096);
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src);

    // Deserialize
    TestObject dst;
    const bool ok = deserializeFromString(buffer.getBuffer(), buffer.size(), &dst);
    CHECK(ok);

    CHECK(dst._intValue == 5);
    CHECK(dst._subObject._names.size() == 2);
    if (dst._subObject._names.size() == 2)
    {
        CHECK(std::strcmp(dst._subObject._names[0].c_str(), "sub_alpha") == 0);
        CHECK(std::strcmp(dst._subObject._names[1].c_str(), "sub_beta")  == 0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// benchmark_Reflect_deserialize_128kb_x100
//   Measures the time taken to deserialize the large_test_object.json file
//   (approximately 128 KB) 100 times using ReflectSerializer::deserializeFromJson.
//
//   Since large_test_object.json does not match the TestObject schema (it has
//   keys that TestObject doesn't register), unknown keys are silently skipped
//   by deserializeObjectFromJson, which is the correct and expected behaviour.
//   The benchmark still exercises the full parse + deserialize path on a
//   realistically-sized file.
// ─────────────────────────────────────────────────────────────────────────────
void benchmark_Reflect_deserialize_128kb_x100()
{
    printSection("Reflect - deserialize 128 KB JSON x100 (benchmark)");

    const char* kFilePath = "large_test_object.json";

    File file;
    const bool loaded = file.load(kFilePath);
    CHECK(loaded);
    if (!loaded)
    {
        std::printf("  [SKIP] Could not load %s\n", kFilePath);
        return;
    }

    const char*  jsonBuffer = file.getStringBuffer();
    const size_t jsonSize   = file.getFileSize();
    const double fileSizeKb = static_cast<double>(jsonSize) / 1024.0;

    std::printf("  file: %s\n", kFilePath);
    std::printf("  size: %.1f KB\n", fileSizeKb);

    constexpr int kIterations = 100;
    int successCount = 0;

    const auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < kIterations; ++i)
    {
        JsonDocument doc;
        if (!doc.buildFromJsonString(jsonBuffer, jsonSize))
            continue;

        JsonObject root = doc.getRootObject();
        if (!root.isValid())
            continue;

        TestObject obj;
        ReflectSerializer::deserializeObjectFromJson(root, &obj);
        ++successCount;
    }

    const auto end = std::chrono::high_resolution_clock::now();

    const double elapsedMs      = std::chrono::duration<double, std::milli>(end - start).count();
    const double avgMsPerIter   = elapsedMs / static_cast<double>(kIterations);
    const double itersPerSec    = static_cast<double>(kIterations) / (elapsedMs / 1000.0);
    const double dataRateMbPerS = (fileSizeKb / 1024.0) * itersPerSec;

    std::printf("  iterations:      %d\n", kIterations);
    std::printf("  success:         %d\n", successCount);
    std::printf("  total elapsed:   %.3f ms\n", elapsedMs);
    std::printf("  avg per iter:    %.3f ms\n", avgMsPerIter);
    std::printf("  throughput:      %.1f iters/sec\n", itersPerSec);
    std::printf("  data rate:       %.1f MB/s\n", dataRateMbPerS);

    CHECK(successCount == kIterations);
}

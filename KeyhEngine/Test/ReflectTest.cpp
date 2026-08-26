#include "TestCommon.h"
#include "ReflectTest.h"
#include "test.h"
#include "ReflectSerializer.h"
#include "ReflectPropertyPolicy.h"
#include "DynamicBuffer.h"
#include "JsonDocument.h"
#include "File.h"

#include <chrono>
#include <cstring>
#include <cstdio>

using namespace keyh;

namespace keyh
{
    enum class EReflectEnumJsonTest
    {
        Int,
        Float3
    };

    KEYH_REFLECT_ENUM_BEGIN(EReflectEnumJsonTest)
        KEYH_REFLECT_ENUM_VALUE(EReflectEnumJsonTest, Int)
        KEYH_REFLECT_ENUM_VALUE(EReflectEnumJsonTest, Float3)
        KEYH_REFLECT_ENUM_END(EReflectEnumJsonTest)
        KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EReflectEnumJsonTest, Int)
        KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EReflectEnumJsonTest, Float3)
    KEYH_REFLECT_ENUM_END_FROM_STRING()
}

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

    static bool deserializeFromFile(const char* filePath, IReflectObject* obj)
    {
        File file;
        if (!file.load(filePath))
            return false;

        return deserializeFromString(file.getStringBuffer(), file.getFileSize(), obj);
    }

    static bool containsToken(const char* data, size_t size, const char* token)
    {
        const size_t tokenLen = std::strlen(token);
        if (tokenLen == 0 || tokenLen > size)
            return false;

        for (size_t i = 0; i + tokenLen <= size; ++i)
        {
            if (std::memcmp(data + i, token, tokenLen) == 0)
                return true;
        }
        return false;
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
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src, 0, true);

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
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src, 0, true);

    // Deserialize
    TestObject dst;
    const bool ok = deserializeFromString(buffer.getBuffer(), buffer.size(), &dst);
    CHECK(ok);

    CHECK(containsToken(buffer.getBuffer(), buffer.size(), "\n  \""));

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
    ReflectSerializer::serializeObjectToBuffer(&buffer, &src, 0, true);

    // Deserialize
    TestObject dst;
    const bool ok = deserializeFromString(buffer.getBuffer(), buffer.size(), &dst);
    CHECK(ok);

    CHECK(containsToken(buffer.getBuffer(), buffer.size(), "\n    \""));

    CHECK(dst._intValue == 5);
    CHECK(dst._subObject._names.size() == 2);
    if (dst._subObject._names.size() == 2)
    {
        CHECK(std::strcmp(dst._subObject._names[0].c_str(), "sub_alpha") == 0);
        CHECK(std::strcmp(dst._subObject._names[1].c_str(), "sub_beta")  == 0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_large_object_file_roundtrip_compare
//   Loads large_test_object.json into TestObject A, serializes A to
//   large_test_object_test.json, loads that file into TestObject B, and checks
//   that the serialized JSON bytes of A and B are byte-for-byte identical.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_large_object_file_roundtrip_compare()
{
    printSection("Reflect - large object file round-trip compare");

    const char* kSourceFilePath = "large_test_object.json";
    const char* kRoundTripFilePath = "large_test_object_test.json";

    TestObject sourceObj;
    const bool loadedSource = deserializeFromFile(kSourceFilePath, &sourceObj);
    CHECK(loadedSource);
    if (!loadedSource)
    {
        std::printf("  [FAIL] Could not load %s\n", kSourceFilePath);
        return;
    }

    const bool serialized = ReflectSerializer::serializeToJson(StringViewA(kRoundTripFilePath), &sourceObj);
    CHECK(serialized);
    if (!serialized)
    {
        std::printf("  [FAIL] Could not write %s\n", kRoundTripFilePath);
        return;
    }

    TestObject roundTripObj;
    const bool loadedRoundTrip = deserializeFromFile(kRoundTripFilePath, &roundTripObj);
    CHECK(loadedRoundTrip);
    if (!loadedRoundTrip)
    {
        std::printf("  [FAIL] Could not load %s\n", kRoundTripFilePath);
        return;
    }

    // Serialize both objects to in-memory buffers and compare byte-for-byte.
    // 2 MB is large enough to hold the TestObject schema serialized from
    // large_test_object.json (the source file is ~512 KB and the schema covers
    // all registered fields including the new Nested/Deep hierarchy).
    static constexpr size_t kSerializeBufferSize = 2 * 1024 * 1024;

    ReflectBufferProxy srcBuffer;
    srcBuffer.allocate(kSerializeBufferSize);
    ReflectSerializer::serializeObjectToBuffer(&srcBuffer, &sourceObj);

    ReflectBufferProxy rtBuffer;
    rtBuffer.allocate(kSerializeBufferSize);
    ReflectSerializer::serializeObjectToBuffer(&rtBuffer, &roundTripObj);

    const bool sameSize = (srcBuffer.size() == rtBuffer.size());
    CHECK(sameSize);
    if (!sameSize)
    {
        std::printf("  [FAIL] Serialized sizes differ: source=%zu roundtrip=%zu\n",
            srcBuffer.size(), rtBuffer.size());
        return;
    }

    const bool sameContent = (std::memcmp(srcBuffer.getBuffer(), rtBuffer.getBuffer(), srcBuffer.size()) == 0);
    CHECK(sameContent);
    if (!sameContent)
        std::printf("  [FAIL] Serialized file contents differ between %s and %s\n",
            kSourceFilePath, kRoundTripFilePath);
}

// ─────────────────────────────────────────────────────────────────────────────
// test_Reflect_enum_string_roundtrip
//   Verifies that registered enums are serialized as string tokens and
//   deserialized back from both string and integer JSON forms.
// ─────────────────────────────────────────────────────────────────────────────
void test_Reflect_enum_string_roundtrip()
{
    printSection("Reflect - enum string serialize/deserialize");

    EReflectEnumJsonTest srcValue = EReflectEnumJsonTest::Float3;

    ReflectBufferProxy buffer;
    buffer.allocate(128);
    ReflectPropertyPolicy<EReflectEnumJsonTest>::serializeToJson(&buffer, srcValue);

    CHECK(containsToken(buffer.getBuffer(), buffer.size(), "\"Float3\""));

    const char* stringJson = "{\"Value\":\"Int\"}";
    JsonDocument stringDoc;
    CHECK(stringDoc.buildFromJsonString(stringJson, std::strlen(stringJson)));
    JsonObject stringRoot = stringDoc.getRootObject();
    JsonKey stringKey = stringRoot.getFirstKey();
    JsonValue stringValue = stringKey.getValue();

    EReflectEnumJsonTest fromString = EReflectEnumJsonTest::Float3;
    ReflectPropertyPolicy<EReflectEnumJsonTest>::deserializeFromJson(stringValue, fromString);
    CHECK(fromString == EReflectEnumJsonTest::Int);

    const char* intJson = "{\"Value\":1}";
    JsonDocument intDoc;
    CHECK(intDoc.buildFromJsonString(intJson, std::strlen(intJson)));
    JsonObject intRoot = intDoc.getRootObject();
    JsonKey intKey = intRoot.getFirstKey();
    JsonValue intValue = intKey.getValue();

    EReflectEnumJsonTest fromInt = EReflectEnumJsonTest::Int;
    ReflectPropertyPolicy<EReflectEnumJsonTest>::deserializeFromJson(intValue, fromInt);
    CHECK(fromInt == EReflectEnumJsonTest::Float3);
}

// ─────────────────────────────────────────────────────────────────────────────
// benchmark_Reflect_deserialize_512kb_x100
//   Measures the time taken to deserialize the large_test_object.json file
//   (approximately 512 KB) 100 times using ReflectSerializer::deserializeFromJson.
//
//   large_test_object.json is structured to match the TestObject schema,
//   including the new Nested (TestNestedObject) and Deep (TestDeepObject)
//   hierarchy. All registered fields are exercised on each iteration.
// ─────────────────────────────────────────────────────────────────────────────
void benchmark_Reflect_deserialize_512kb_x100()
{
    printSection("Reflect - deserialize 512 KB JSON x100 (benchmark)");

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

#include "TestCommon.h"
#include "JsonTest.h"
#include "JsonDocument.h"
#include "JsonElement.h"
#include "JsonUtil.h"
#include "File.h"

#include <chrono>
#include <cstdio>
#include <cstring>

using namespace keyh;

// ─────────────────────────────────────────────────────────────────────────────
// Internal traversal helpers
// ─────────────────────────────────────────────────────────────────────────────
namespace
{
    static bool svEqual(const StringViewA& sv, const char* str)
    {
        const size_t len = static_cast<size_t>(std::strlen(str));
        return sv.length() == len && std::memcmp(sv.data(), str, len) == 0;
    }

    // Forward declarations for mutual recursion
    static void traverseObject(const JsonObject& obj,
                               bool& strFound, const char* strTarget,
                               bool& intFound, int intTarget,
                               bool& boolFound, bool boolTarget,
                               size_t& nodeCount);

    static void traverseArray(const JsonArray& arr,
                              bool& strFound, const char* strTarget,
                              bool& intFound, int intTarget,
                              bool& boolFound, bool boolTarget,
                              size_t& nodeCount);

    static void traverseValue(const JsonValue& v,
                              bool& strFound, const char* strTarget,
                              bool& intFound, int intTarget,
                              bool& boolFound, bool boolTarget,
                              size_t& nodeCount)
    {
        ++nodeCount;
        using TT = JsonUtil::TapeType;
        switch (v.getValueType())
        {
        case TT::String:
            if (svEqual(v.getStringValue(), strTarget))
                strFound = true;
            break;
        case TT::Integer:
            if (v.getIntValue() == intTarget)
                intFound = true;
            break;
        case TT::Boolean:
            if (v.getBoolValue() == boolTarget)
                boolFound = true;
            break;
        case TT::ObjectStart:
            traverseObject(v.getObjectValue(),
                           strFound, strTarget, intFound, intTarget, boolFound, boolTarget, nodeCount);
            break;
        case TT::ArrayStart:
            traverseArray(v.getArrayValue(),
                          strFound, strTarget, intFound, intTarget, boolFound, boolTarget, nodeCount);
            break;
        default:
            break;
        }
    }

    static void traverseObject(const JsonObject& obj,
                               bool& strFound, const char* strTarget,
                               bool& intFound, int intTarget,
                               bool& boolFound, bool boolTarget,
                               size_t& nodeCount)
    {
        for (JsonKey k = obj.getFirstKey(); k.isValid(); k = obj.getNextKey(k))
        {
            ++nodeCount; // the key itself is a node
            traverseValue(k.getValue(),
                          strFound, strTarget, intFound, intTarget, boolFound, boolTarget, nodeCount);
        }
    }

    static void traverseArray(const JsonArray& arr,
                              bool& strFound, const char* strTarget,
                              bool& intFound, int intTarget,
                              bool& boolFound, bool boolTarget,
                              size_t& nodeCount)
    {
        for (JsonValue v = arr.getFirstValue(); v.isValid(); v = arr.getNextValue(v))
        {
            traverseValue(v,
                          strFound, strTarget, intFound, intTarget, boolFound, boolTarget, nodeCount);
        }
    }

    static bool containsStringToken(const JsonDocument& doc, const char* expected)
    {
        bool strFound = false, intFound = false, boolFound = false;
        size_t nodeCount = 0;
        traverseObject(doc.getRootObject(),
                       strFound, expected, intFound, 0, boolFound, false, nodeCount);
        return strFound;
    }

    static bool containsIntegerToken(const JsonDocument& doc, int expected)
    {
        bool strFound = false, intFound = false, boolFound = false;
        size_t nodeCount = 0;
        traverseObject(doc.getRootObject(),
                       strFound, "", intFound, expected, boolFound, false, nodeCount);
        return intFound;
    }

    static bool containsBooleanToken(const JsonDocument& doc, bool expected)
    {
        bool strFound = false, intFound = false, boolFound = false;
        size_t nodeCount = 0;
        traverseObject(doc.getRootObject(),
                       strFound, "", intFound, 0, boolFound, expected, nodeCount);
        return boolFound;
    }

    // ─── Count-only traversal for benchmarking ───────────────────────────────
    static size_t countValueNodes(const JsonValue& v);
    static size_t countObjectNodes(const JsonObject& obj);
    static size_t countArrayNodes(const JsonArray& arr);

    static size_t countValueNodes(const JsonValue& v)
    {
        using TT = JsonUtil::TapeType;
        switch (v.getValueType())
        {
        case TT::ObjectStart: return 1 + countObjectNodes(v.getObjectValue());
        case TT::ArrayStart:  return 1 + countArrayNodes(v.getArrayValue());
        default:              return 1;
        }
    }

    static size_t countObjectNodes(const JsonObject& obj)
    {
        size_t count = 0;
        for (JsonKey k = obj.getFirstKey(); k.isValid(); k = obj.getNextKey(k))
            count += 1 + countValueNodes(k.getValue()); // +1 for the key string itself
        return count;
    }

    static size_t countArrayNodes(const JsonArray& arr)
    {
        size_t count = 0;
        for (JsonValue v = arr.getFirstValue(); v.isValid(); v = arr.getNextValue(v))
            count += countValueNodes(v);
        return count;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Correctness tests
// ─────────────────────────────────────────────────────────────────────────────

void test_Json_parse_test_file()
{
    printSection("Json - parse test.json");

    JsonDocument document("test.json");
    CHECK(document.isValid());
    CHECK(containsStringToken(document, "KEEngine"));
    CHECK(containsStringToken(document, "GaussianSplatComponent"));
    CHECK(containsIntegerToken(document, 3840));
    CHECK(containsIntegerToken(document, 2160));
    CHECK(containsBooleanToken(document, true));
    CHECK(containsBooleanToken(document, false));
}

void test_Json_traverse_root_object()
{
    printSection("Json - root object key count");

    JsonDocument document("test.json");
    CHECK(document.isValid());
    if (!document.isValid())
        return;

    // test.json root has exactly 6 keys:
    //   engine_metadata, graphics_settings, reflection_system,
    //   render_pipeline, resource_management, scene_data
    size_t rootKeyCount = 0;
    JsonObject root = document.getRootObject();
    for (JsonKey k = root.getFirstKey(); k.isValid(); k = root.getNextKey(k))
        ++rootKeyCount;

    std::printf("  root key count: %zu (expected 6)\n", rootKeyCount);
    CHECK(rootKeyCount == 6);

    // Verify the first key name is "engine_metadata"
    JsonKey firstKey = root.getFirstKey();
    CHECK(firstKey.isValid());
    if (firstKey.isValid())
        CHECK(svEqual(firstKey.getKeyName(), "engine_metadata"));
}

void test_Json_traverse_array()
{
    printSection("Json - array element traversal");

    JsonDocument document("test.json");
    CHECK(document.isValid());
    if (!document.isValid())
        return;

    // Locate graphics_settings.feature_levels array and count its 3 string elements
    JsonObject root      = document.getRootObject();
    JsonKey    gsKey     = root.getFirstKey(); // engine_metadata
    gsKey                = root.getNextKey(gsKey); // graphics_settings
    CHECK(gsKey.isValid() && svEqual(gsKey.getKeyName(), "graphics_settings"));

    JsonObject gsObj     = gsKey.getValue().getObjectValue();

    // Iterate keys of graphics_settings to find feature_levels
    size_t featureLevelCount = 0;
    for (JsonKey k = gsObj.getFirstKey(); k.isValid(); k = gsObj.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "feature_levels"))
        {
            JsonArray arr = k.getValue().getArrayValue();
            for (JsonValue v = arr.getFirstValue(); v.isValid(); v = arr.getNextValue(v))
                ++featureLevelCount;
            break;
        }
    }
    std::printf("  feature_levels element count: %zu (expected 3)\n", featureLevelCount);
    CHECK(featureLevelCount == 3);

    // Verify we can read the first element of the array
    for (JsonKey k = gsObj.getFirstKey(); k.isValid(); k = gsObj.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "feature_levels"))
        {
            JsonArray arr  = k.getValue().getArrayValue();
            JsonValue first = arr.getFirstValue();
            CHECK(first.isValid());
            if (first.isValid())
                CHECK(svEqual(first.getStringValue(), "D3D_FEATURE_LEVEL_12_0"));
            break;
        }
    }
}

void test_Json_traverse_nested_value()
{
    printSection("Json - nested value access");

    JsonDocument document("test.json");
    CHECK(document.isValid());
    if (!document.isValid())
        return;

    // graphics_settings.resolution.width == 3840
    // graphics_settings.resolution.height == 2160
    JsonObject root  = document.getRootObject();
    JsonObject gsObj;
    for (JsonKey k = root.getFirstKey(); k.isValid(); k = root.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "graphics_settings"))
        {
            gsObj = k.getValue().getObjectValue();
            break;
        }
    }
    CHECK(gsObj.isValid());

    JsonObject resObj;
    for (JsonKey k = gsObj.getFirstKey(); k.isValid(); k = gsObj.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "resolution"))
        {
            resObj = k.getValue().getObjectValue();
            break;
        }
    }
    CHECK(resObj.isValid());

    int width = 0, height = 0;
    for (JsonKey k = resObj.getFirstKey(); k.isValid(); k = resObj.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "width"))
            width = k.getValue().getIntValue();
        else if (svEqual(k.getKeyName(), "height"))
            height = k.getValue().getIntValue();
    }
    std::printf("  resolution: %dx%d (expected 3840x2160)\n", width, height);
    CHECK(width == 3840);
    CHECK(height == 2160);
}

void test_Json_traverse_empty_containers()
{
    printSection("Json - empty array and object traversal");

    // Build a minimal JSON with an empty array and an empty object
    const char* jsonStr =
        "{"
        "  \"empty_arr\": [],"
        "  \"empty_obj\": {},"
        "  \"value\": 42"
        "}";

    JsonDocument document;
    CHECK(document.buildFromJsonString(jsonStr, std::strlen(jsonStr)));
    if (!document.isValid())
        return;

    JsonObject root = document.getRootObject();
    size_t keyCount = 0;
    for (JsonKey k = root.getFirstKey(); k.isValid(); k = root.getNextKey(k))
    {
        ++keyCount;
        if (svEqual(k.getKeyName(), "empty_arr"))
        {
            JsonArray arr = k.getValue().getArrayValue();
            size_t elemCount = 0;
            for (JsonValue v = arr.getFirstValue(); v.isValid(); v = arr.getNextValue(v))
                ++elemCount;
            std::printf("  empty_arr element count: %zu (expected 0)\n", elemCount);
            CHECK(elemCount == 0);
        }
        else if (svEqual(k.getKeyName(), "empty_obj"))
        {
            JsonObject obj = k.getValue().getObjectValue();
            size_t innerKeyCount = 0;
            for (JsonKey ik = obj.getFirstKey(); ik.isValid(); ik = obj.getNextKey(ik))
                ++innerKeyCount;
            std::printf("  empty_obj key count: %zu (expected 0)\n", innerKeyCount);
            CHECK(innerKeyCount == 0);
        }
    }
    std::printf("  root key count: %zu (expected 3)\n", keyCount);
    CHECK(keyCount == 3);

    // Confirm "value" key holds 42
    for (JsonKey k = root.getFirstKey(); k.isValid(); k = root.getNextKey(k))
    {
        if (svEqual(k.getKeyName(), "value"))
        {
            CHECK(k.getValue().getIntValue() == 42);
            break;
        }
    }
}

void test_Json_float_value_access()
{
    printSection("Json - float value access");

    // resource_management.xml_indexer.last_parse_time_ms == 18.45 (float)
    JsonDocument document("test.json");
    CHECK(document.isValid());
    if (!document.isValid())
        return;

    JsonObject root = document.getRootObject();
    for (JsonKey k = root.getFirstKey(); k.isValid(); k = root.getNextKey(k))
    {
        if (!svEqual(k.getKeyName(), "resource_management"))
            continue;

        JsonObject rmObj = k.getValue().getObjectValue();
        for (JsonKey k2 = rmObj.getFirstKey(); k2.isValid(); k2 = rmObj.getNextKey(k2))
        {
            if (!svEqual(k2.getKeyName(), "xml_indexer"))
                continue;

            JsonObject xiObj = k2.getValue().getObjectValue();
            for (JsonKey k3 = xiObj.getFirstKey(); k3.isValid(); k3 = xiObj.getNextKey(k3))
            {
                if (svEqual(k3.getKeyName(), "last_parse_time_ms"))
                {
                    const float v = k3.getValue().getFloatValue();
                    std::printf("  last_parse_time_ms: %.4f (expected ~18.45)\n", static_cast<double>(v));
                    // Allow small floating-point tolerance
                    const float diff = v - 18.45f;
                    CHECK(diff > -0.01f && diff < 0.01f);
                    break;
                }
            }
            break;
        }
        break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Benchmarks
// ─────────────────────────────────────────────────────────────────────────────

void benchmark_Json_parse_speed()
{
    printSection("Json - parse performance");

    File file;
    const bool loaded = file.load("test.json");
    CHECK(loaded);
    if (!loaded)
        return;

    const char* jsonBuffer = file.getStringBuffer();
    const size_t jsonSize  = file.getFileSize();

    constexpr int kIterationCount = 1000;
    int successCount = 0;

    const auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kIterationCount; ++i)
    {
        JsonDocument document;
        if (document.buildFromJsonString(jsonBuffer, jsonSize))
            ++successCount;
    }
    const auto end = std::chrono::high_resolution_clock::now();

    const double elapsedMs      = std::chrono::duration<double, std::milli>(end - start).count();
    const double avgUsPerParse  = (elapsedMs * 1000.0) / static_cast<double>(kIterationCount);
    const double parsesPerSecond = static_cast<double>(kIterationCount) / (elapsedMs / 1000.0);
    const double fileSizeMb     = static_cast<double>(jsonSize) / (1024.0 * 1024.0);
    const double parseMbPerSec  = fileSizeMb * parsesPerSecond;

    std::printf("  file size:   %.1f KB\n", static_cast<double>(jsonSize) / 1024.0);
    std::printf("  iterations:  %d\n", kIterationCount);
    std::printf("  elapsed:     %.3f ms\n", elapsedMs);
    std::printf("  avg:         %.3f us/parse\n", avgUsPerParse);
    std::printf("  throughput:  %.1f parses/sec\n", parsesPerSecond);
    std::printf("  data rate:   %.1f MB/s\n", parseMbPerSec);

    CHECK(successCount == kIterationCount);
}

void benchmark_Json_traversal_speed()
{
    printSection("Json - full traversal performance + library comparison");

    File file;
    const bool loaded = file.load("test.json");
    CHECK(loaded);
    if (!loaded)
        return;

    const char*  jsonBuffer = file.getStringBuffer();
    const size_t jsonSize   = file.getFileSize();

    // ── 1. Parse once, measure traversal-only throughput ────────────────────
    JsonDocument parsedDoc;
    CHECK(parsedDoc.buildFromJsonString(jsonBuffer, jsonSize));
    if (!parsedDoc.isValid())
        return;

    const size_t nodesPerDoc = countObjectNodes(parsedDoc.getRootObject());

    constexpr int kTraversalIter = 5000;
    size_t totalNodes = 0;

    const auto tStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kTraversalIter; ++i)
        totalNodes += countObjectNodes(parsedDoc.getRootObject());
    const auto tEnd = std::chrono::high_resolution_clock::now();

    const double tElapsedMs      = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    const double tAvgUsPerTraverse = (tElapsedMs * 1000.0) / static_cast<double>(kTraversalIter);
    const double tTraversalPerSec  = static_cast<double>(kTraversalIter) / (tElapsedMs / 1000.0);
    const double fileSizeMb        = static_cast<double>(jsonSize) / (1024.0 * 1024.0);
    const double tDataRateMbPerSec = fileSizeMb * tTraversalPerSec;

    std::printf("  [Traversal-only]\n");
    std::printf("  nodes per document: %zu\n", nodesPerDoc);
    std::printf("  iterations:         %d\n", kTraversalIter);
    std::printf("  elapsed:            %.3f ms\n", tElapsedMs);
    std::printf("  avg:                %.3f us/traversal\n", tAvgUsPerTraverse);
    std::printf("  data rate:          %.1f MB/s\n", tDataRateMbPerSec);

    // ── 2. Parse + traverse, measuring end-to-end throughput ────────────────
    constexpr int kEndToEndIter = 1000;
    int successCount = 0;

    const auto eStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kEndToEndIter; ++i)
    {
        JsonDocument doc;
        if (doc.buildFromJsonString(jsonBuffer, jsonSize))
        {
            totalNodes += countObjectNodes(doc.getRootObject());
            ++successCount;
        }
    }
    const auto eEnd = std::chrono::high_resolution_clock::now();

    const double eElapsedMs      = std::chrono::duration<double, std::milli>(eEnd - eStart).count();
    const double eAvgUsPerRound  = (eElapsedMs * 1000.0) / static_cast<double>(kEndToEndIter);
    const double eRoundsPerSec   = static_cast<double>(kEndToEndIter) / (eElapsedMs / 1000.0);
    const double eDataRateMbPerSec = fileSizeMb * eRoundsPerSec;

    std::printf("\n  [Parse + full traversal]\n");
    std::printf("  iterations:  %d\n", kEndToEndIter);
    std::printf("  elapsed:     %.3f ms\n", eElapsedMs);
    std::printf("  avg:         %.3f us/round\n", eAvgUsPerRound);
    std::printf("  data rate:   %.1f MB/s\n", eDataRateMbPerSec);

    // ── 3. Library comparison table ──────────────────────────────────────────
    //
    // Published parse+traverse throughput figures (x64 Release, typical datasets):
    //   nlohmann/json  : ~100  MB/s  – header-only DOM, convenience over speed
    //   RapidJSON DOM  : ~350  MB/s  – fast SAX/DOM, avoids memory allocation
    //   simdjson       : ~2500 MB/s  – SIMD-based, requires AVX2; on-demand API
    //                                  reaches ~3 GB/s on native format
    //
    // Sources:
    //   https://github.com/simdjson/simdjson#performance
    //   https://rapidjson.org/md_doc_performance.html
    //   https://github.com/nlohmann/json#benchmarks
    //
    std::printf("\n  --- Comparison with popular JSON libraries ---\n");
    std::printf("  %-28s %10.1f MB/s\n", "KeyhEngine (parse+traverse):", eDataRateMbPerSec);
    std::printf("  %-28s %10s MB/s  (DOM, convenience)\n",   "nlohmann/json:",             "~100");
    std::printf("  %-28s %10s MB/s  (DOM, high performance)\n", "RapidJSON DOM:",           "~350");
    std::printf("  %-28s %10s MB/s  (SIMD, AVX2 required)\n",  "simdjson (on-demand):",    "~2500");
    std::printf("\n");
    std::printf("  Notes:\n");
    std::printf("    - All figures depend heavily on CPU, dataset size/structure,\n");
    std::printf("      compiler flags, and allocation strategy.\n");
    std::printf("    - KeyhEngine uses a tape-based representation; traversal after\n");
    std::printf("      parsing is cache-friendly (sequential index reads).\n");
    std::printf("    - simdjson uses SIMD vectorization unavailable in pure C++20\n");
    std::printf("      scalar code; a fair comparison requires the same ISA.\n");

    CHECK(successCount == kEndToEndIter);
    CHECK(totalNodes > 0);
}

#include "TestCommon.h"
#include "JsonTest.h"
#include "JsonDocument.h"
#include "JsonUtil.h"
#include "File.h"

#include <chrono>
#include <cstdio>
#include <cstring>

using namespace keyh;

namespace
{
    bool containsStringToken(const JsonDocument& json, const char* expected)
    {
        return false;
    }

    bool containsIntegerToken(const JsonDocument& json, int expected)
    {
        return false;
    }

    bool containsBooleanToken(const JsonDocument& json, bool expected)
    {
        return false;
    }
}

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

void benchmark_Json_parse_speed()
{
    printSection("Json - parse performance");

    File file;
    const bool loaded = file.load("test.json");
    CHECK(loaded);
    if (!loaded)
    {
        return;
    }

    const char* jsonBuffer = file.getStringBuffer();
    const size_t jsonSize = file.getFileSize();

    constexpr int kIterationCount = 1000;
    int successCount = 0;
    size_t totalTapeElementCount = 0;

    const auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < kIterationCount; ++i)
    {
        JsonDocument document;
        if (document.buildFromJsonString(jsonBuffer, jsonSize))
        {
            //++successCount;
            //totalTapeElementCount += document.getTapeElementCount();
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();

    const double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();
    const double avgUsPerParse = (elapsedMs * 1000.0) / static_cast<double>(kIterationCount);
    const double parsesPerSecond = static_cast<double>(kIterationCount) / (elapsedMs / 1000.0);

    std::printf("  iterations: %d\n", kIterationCount);
    std::printf("  elapsed: %.3f ms\n", elapsedMs);
    std::printf("  avg: %.3f us/parse\n", avgUsPerParse);
    std::printf("  throughput: %.1f parses/sec\n", parsesPerSecond);
    std::printf("  avg tape elements: %.1f\n", static_cast<double>(totalTapeElementCount) / static_cast<double>(kIterationCount));

    CHECK(successCount == kIterationCount);
}

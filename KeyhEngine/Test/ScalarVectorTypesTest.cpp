#include "TestCommon.h"
#include "ScalarVectorTypesTest.h"
#include "ScalarVectorTypes.h"

#include <chrono>

using namespace keyh;

namespace
{
    volatile double g_scalarVectorBenchmarkSink = 0.0;

    template <typename Fn>
    double measureMilliseconds(Fn&& fn)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        fn();
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
}

void test_ScalarVectorTypes_float2_basic()
{
    printSection("ScalarVectorTypes - float2");

    float2 left(2.0f, 4.0f);
    float2 right(3.0f, 5.0f);

    CHECK((left + right) == float2(5.0f, 9.0f));
    CHECK((right - left) == float2(1.0f, 1.0f));
    CHECK((left * right) == float2(6.0f, 20.0f));
    CHECK((right / left) == float2(1.5f, 1.25f));
    CHECK((left + 1.0f) == float2(3.0f, 5.0f));
    CHECK((2.0f * right) == float2(6.0f, 10.0f));

    left += right;
    CHECK(left == float2(5.0f, 9.0f));

    left -= 2.0f;
    CHECK(left == float2(3.0f, 7.0f));
    CHECK(left[0] == 3.0f);
    CHECK(left[1] == 7.0f);
}

void test_ScalarVectorTypes_float3_basic()
{
    printSection("ScalarVectorTypes - float3");

    float3 left(1.0f, 2.0f, 3.0f);
    float3 right(4.0f, 5.0f, 6.0f);

    CHECK((left + right) == float3(5.0f, 7.0f, 9.0f));
    CHECK((right - left) == float3(3.0f, 3.0f, 3.0f));
    CHECK((left * right) == float3(4.0f, 10.0f, 18.0f));
    CHECK((right / left) == float3(4.0f, 2.5f, 2.0f));
    CHECK((left + 2.0f) == float3(3.0f, 4.0f, 5.0f));
    CHECK(left.dot(right) == 32.0f);
    CHECK(float3(1.0f, 0.0f, 0.0f).cross(float3(0.0f, 1.0f, 0.0f)) == float3(0.0f, 0.0f, 1.0f));

    right /= 2.0f;
    CHECK(right == float3(2.0f, 2.5f, 3.0f));
    CHECK((-left) == float3(-1.0f, -2.0f, -3.0f));
}

void test_ScalarVectorTypes_float4_basic()
{
    printSection("ScalarVectorTypes - float4");

    float3 xyz(1.0f, 2.0f, 3.0f);
    float4 left(xyz, 4.0f);
    float4 right(5.0f, 6.0f, 7.0f, 8.0f);

    CHECK(left == float4(1.0f, 2.0f, 3.0f, 4.0f));
    CHECK((left + right) == float4(6.0f, 8.0f, 10.0f, 12.0f));
    CHECK((right - left) == float4(4.0f, 4.0f, 4.0f, 4.0f));
    CHECK((left * 2.0f) == float4(2.0f, 4.0f, 6.0f, 8.0f));
    CHECK((right / 2.0f) == float4(2.5f, 3.0f, 3.5f, 4.0f));

    left += 1.0f;
    CHECK(left == float4(2.0f, 3.0f, 4.0f, 5.0f));
    CHECK(left[2] == 4.0f);
    CHECK(left[3] == 5.0f);
}

void benchmark_ScalarVectorTypes_arithmetic()
{
    printSection("ScalarVectorTypes - arithmetic performance");

    std::printf("  SIMD path:   %s\n", KEYH_SCALAR_VECTOR_SIMD_ENABLED ? "enabled" : "disabled");
    std::printf("  toggle macro: KEYH_SCALAR_VECTOR_USE_SIMD\n");

    constexpr int kIterationCount = 1000000;

    const double float2ElapsedMs = measureMilliseconds([]
    {
        float2 current(1.0f, 2.0f);
        const float2 addValue(0.25f, 0.5f);
        const float2 mulValue(0.9999f, 0.9998f);
        const float2 divValue(1.0001f, 1.0002f);
        const float2 subValue(0.125f, 0.25f);

        for (int index = 0; index < kIterationCount; ++index)
        {
            current = (((current + addValue) * mulValue) / divValue) - subValue;
            g_scalarVectorBenchmarkSink += current.x + current.y;
        }
    });

    const double float3ElapsedMs = measureMilliseconds([]
    {
        float3 current(1.0f, 2.0f, 3.0f);
        const float3 addValue(0.1f, 0.2f, 0.3f);
        const float3 other(3.0f, 4.0f, 5.0f);

        for (int index = 0; index < kIterationCount; ++index)
        {
            current = (current + addValue) * 0.9999f;
            g_scalarVectorBenchmarkSink += current.dot(other);

            const float3 crossValue = current.cross(other);
            g_scalarVectorBenchmarkSink += crossValue.x + crossValue.y + crossValue.z;
        }
    });

    const double float4ElapsedMs = measureMilliseconds([]
    {
        float4 current(1.0f, 2.0f, 3.0f, 4.0f);
        const float4 addValue(0.5f, 0.75f, 1.0f, 1.25f);
        const float4 mulValue(0.9998f, 0.9997f, 0.9996f, 0.9995f);
        const float4 subValue(0.2f, 0.3f, 0.4f, 0.5f);

        for (int index = 0; index < kIterationCount; ++index)
        {
            current = ((current + addValue) * mulValue) - subValue;
            g_scalarVectorBenchmarkSink += current.x + current.y + current.z + current.w;
        }
    });

    std::printf("  iterations:  %d\n", kIterationCount);
    std::printf("  float2:      %.3f ms\n", float2ElapsedMs);
    std::printf("  float3:      %.3f ms\n", float3ElapsedMs);
    std::printf("  float4:      %.3f ms\n", float4ElapsedMs);
    std::printf("  sink:        %.3f\n", g_scalarVectorBenchmarkSink);
}

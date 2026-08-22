#pragma once

#include <cassert>
#include <cstdlib>

#include "CommonCore.h"

#if defined(KEYH_SCALAR_VECTOR_USE_SIMD) && ((defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__SSE__) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)))
#include <xmmintrin.h>
#define KEYH_SCALAR_VECTOR_SIMD_ENABLED 1
#elif defined(KEYH_SCALAR_VECTOR_USE_SIMD)
#error KEYH_SCALAR_VECTOR_USE_SIMD requires an SSE-capable x86/x64 target.
#else
#define KEYH_SCALAR_VECTOR_SIMD_ENABLED 0
#endif

namespace keyh
{
    [[noreturn]] inline void scalarVectorIndexOutOfBounds() noexcept
    {
        assert(false && "scalar vector index out of bounds");
        std::abort();

#if defined(_MSC_VER)
        __assume(0);
#elif defined(__GNUC__) || defined(__clang__)
        __builtin_unreachable();
#endif
    }

    struct float2
    {
        float x = 0.0f;
        float y = 0.0f;

        constexpr float2() noexcept = default;
        constexpr explicit float2(float value) noexcept
            : x(value)
            , y(value)
        {
        }

        constexpr float2(float xValue, float yValue) noexcept
            : x(xValue)
            , y(yValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        const float& operator[](size_t index) const noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        float2 operator+(const float2& other) const noexcept
        {
            return float2(x + other.x, y + other.y);
        }

        float2 operator-(const float2& other) const noexcept
        {
            return float2(x - other.x, y - other.y);
        }

        float2 operator*(const float2& other) const noexcept
        {
            return float2(x * other.x, y * other.y);
        }

        float2 operator/(const float2& other) const noexcept
        {
            return float2(x / other.x, y / other.y);
        }

        float2 operator+(float scalar) const noexcept
        {
            return *this + float2(scalar);
        }

        float2 operator-(float scalar) const noexcept
        {
            return *this - float2(scalar);
        }

        float2 operator*(float scalar) const noexcept
        {
            return *this * float2(scalar);
        }

        float2 operator/(float scalar) const noexcept
        {
            return *this / float2(scalar);
        }

        float2& operator+=(const float2& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        float2& operator-=(const float2& other) noexcept
        {
            *this = *this - other;
            return *this;
        }

        float2& operator*=(const float2& other) noexcept
        {
            *this = *this * other;
            return *this;
        }

        float2& operator/=(const float2& other) noexcept
        {
            *this = *this / other;
            return *this;
        }

        float2& operator+=(float scalar) noexcept
        {
            *this = *this + scalar;
            return *this;
        }

        float2& operator-=(float scalar) noexcept
        {
            *this = *this - scalar;
            return *this;
        }

        float2& operator*=(float scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        float2& operator/=(float scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }

        float2 operator-() const noexcept
        {
            return float2(-x, -y);
        }

        bool operator==(const float2& other) const noexcept
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const float2& other) const noexcept
        {
            return !(*this == other);
        }

        friend float2 operator+(float scalar, const float2& value) noexcept
        {
            return value + scalar;
        }

        friend float2 operator-(float scalar, const float2& value) noexcept
        {
            return float2(scalar) - value;
        }

        friend float2 operator*(float scalar, const float2& value) noexcept
        {
            return value * scalar;
        }

        friend float2 operator/(float scalar, const float2& value) noexcept
        {
            return float2(scalar) / value;
        }
    };

    struct float3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        constexpr float3() noexcept = default;
        constexpr explicit float3(float value) noexcept
            : x(value)
            , y(value)
            , z(value)
        {
        }

        constexpr float3(float xValue, float yValue, float zValue) noexcept
            : x(xValue)
            , y(yValue)
            , z(zValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        const float& operator[](size_t index) const noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        float3 operator+(const float3& other) const noexcept
        {
            return float3(x + other.x, y + other.y, z + other.z);
        }

        float3 operator-(const float3& other) const noexcept
        {
            return float3(x - other.x, y - other.y, z - other.z);
        }

        float3 operator*(const float3& other) const noexcept
        {
            return float3(x * other.x, y * other.y, z * other.z);
        }

        float3 operator/(const float3& other) const noexcept
        {
            return float3(x / other.x, y / other.y, z / other.z);
        }

        float3 operator+(float scalar) const noexcept
        {
            return *this + float3(scalar);
        }

        float3 operator-(float scalar) const noexcept
        {
            return *this - float3(scalar);
        }

        float3 operator*(float scalar) const noexcept
        {
            return *this * float3(scalar);
        }

        float3 operator/(float scalar) const noexcept
        {
            return *this / float3(scalar);
        }

        float3& operator+=(const float3& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        float3& operator-=(const float3& other) noexcept
        {
            *this = *this - other;
            return *this;
        }

        float3& operator*=(const float3& other) noexcept
        {
            *this = *this * other;
            return *this;
        }

        float3& operator/=(const float3& other) noexcept
        {
            *this = *this / other;
            return *this;
        }

        float3& operator+=(float scalar) noexcept
        {
            *this = *this + scalar;
            return *this;
        }

        float3& operator-=(float scalar) noexcept
        {
            *this = *this - scalar;
            return *this;
        }

        float3& operator*=(float scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        float3& operator/=(float scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }

        float3 operator-() const noexcept
        {
            return float3(-x, -y, -z);
        }

        float dot(const float3& other) const noexcept
        {
            return x * other.x + y * other.y + z * other.z;
        }

        float3 cross(const float3& other) const noexcept
        {
            return float3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }

        bool operator==(const float3& other) const noexcept
        {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const float3& other) const noexcept
        {
            return !(*this == other);
        }

        friend float3 operator+(float scalar, const float3& value) noexcept
        {
            return value + scalar;
        }

        friend float3 operator-(float scalar, const float3& value) noexcept
        {
            return float3(scalar) - value;
        }

        friend float3 operator*(float scalar, const float3& value) noexcept
        {
            return value * scalar;
        }

        friend float3 operator/(float scalar, const float3& value) noexcept
        {
            return float3(scalar) / value;
        }
    };

    struct alignas(16) float4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        constexpr float4() noexcept = default;
        constexpr explicit float4(float value) noexcept
            : x(value)
            , y(value)
            , z(value)
            , w(value)
        {
        }

        constexpr float4(float xValue, float yValue, float zValue, float wValue) noexcept
            : x(xValue)
            , y(yValue)
            , z(zValue)
            , w(wValue)
        {
        }

        constexpr float4(const float3& xyzValue, float wValue) noexcept
            : x(xyzValue.x)
            , y(xyzValue.y)
            , z(xyzValue.z)
            , w(wValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        const float& operator[](size_t index) const noexcept
        {
            switch (index)
            {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                scalarVectorIndexOutOfBounds();
            }
        }

        float4 operator+(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result.x, _mm_add_ps(_mm_load_ps(&x), _mm_load_ps(&other.x)));
            return result;
#else
            return float4(x + other.x, y + other.y, z + other.z, w + other.w);
#endif
        }

        float4 operator-(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result.x, _mm_sub_ps(_mm_load_ps(&x), _mm_load_ps(&other.x)));
            return result;
#else
            return float4(x - other.x, y - other.y, z - other.z, w - other.w);
#endif
        }

        float4 operator*(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result.x, _mm_mul_ps(_mm_load_ps(&x), _mm_load_ps(&other.x)));
            return result;
#else
            return float4(x * other.x, y * other.y, z * other.z, w * other.w);
#endif
        }

        float4 operator/(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result.x, _mm_div_ps(_mm_load_ps(&x), _mm_load_ps(&other.x)));
            return result;
#else
            return float4(x / other.x, y / other.y, z / other.z, w / other.w);
#endif
        }

        float4 operator+(float scalar) const noexcept
        {
            return *this + float4(scalar);
        }

        float4 operator-(float scalar) const noexcept
        {
            return *this - float4(scalar);
        }

        float4 operator*(float scalar) const noexcept
        {
            return *this * float4(scalar);
        }

        float4 operator/(float scalar) const noexcept
        {
            return *this / float4(scalar);
        }

        float4& operator+=(const float4& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        float4& operator-=(const float4& other) noexcept
        {
            *this = *this - other;
            return *this;
        }

        float4& operator*=(const float4& other) noexcept
        {
            *this = *this * other;
            return *this;
        }

        float4& operator/=(const float4& other) noexcept
        {
            *this = *this / other;
            return *this;
        }

        float4& operator+=(float scalar) noexcept
        {
            *this = *this + scalar;
            return *this;
        }

        float4& operator-=(float scalar) noexcept
        {
            *this = *this - scalar;
            return *this;
        }

        float4& operator*=(float scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        float4& operator/=(float scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }

        float4 operator-() const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            const __m128 result = _mm_xor_ps(_mm_setr_ps(x, y, z, w), _mm_set1_ps(-0.0f));

            float values[4];
            _mm_storeu_ps(values, result);
            return float4(values[0], values[1], values[2], values[3]);
#else
            return float4(-x, -y, -z, -w);
#endif
        }

        bool operator==(const float4& other) const noexcept
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        bool operator!=(const float4& other) const noexcept
        {
            return !(*this == other);
        }

        friend float4 operator+(float scalar, const float4& value) noexcept
        {
            return value + scalar;
        }

        friend float4 operator-(float scalar, const float4& value) noexcept
        {
            return float4(scalar) - value;
        }

        friend float4 operator*(float scalar, const float4& value) noexcept
        {
            return value * scalar;
        }

        friend float4 operator/(float scalar, const float4& value) noexcept
        {
            return float4(scalar) / value;
        }
    };

    inline float dot(const float3& left, const float3& right) noexcept
    {
        return left.dot(right);
    }

    inline float3 cross(const float3& left, const float3& right) noexcept
    {
        return left.cross(right);
    }
}

#pragma once

#include "AssertUtil.h"
#include "CommonCore.h"

//#define KEYH_SCALAR_VECTOR_USE_SIMD

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
    struct float2
    {
        union
        {
            struct { float _x; float _y; };
            float _value[2];
        };

        constexpr float2() noexcept : _x(0.0f), _y(0.0f) {}
        constexpr explicit float2(float value) noexcept
            : _x(value)
            , _y(value)
        {
        }

        constexpr float2(float xValue, float yValue) noexcept
            : _x(xValue)
            , _y(yValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        const float& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        float2 operator+(const float2& other) const noexcept
        {
            return float2(_x + other._x, _y + other._y);
        }

        float2 operator-(const float2& other) const noexcept
        {
            return float2(_x - other._x, _y - other._y);
        }

        float2 operator*(const float2& other) const noexcept
        {
            return float2(_x * other._x, _y * other._y);
        }

        float2 operator/(const float2& other) const noexcept
        {
            return float2(_x / other._x, _y / other._y);
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
            return float2(-_x, -_y);
        }

        bool operator==(const float2& other) const noexcept
        {
            return _x == other._x && _y == other._y;
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
        union
        {
            struct { float _x; float _y; float _z; };
            float _value[3];
        };

        constexpr float3() noexcept : _x(0.0f), _y(0.0f), _z(0.0f) {}
        constexpr explicit float3(float value) noexcept
            : _x(value)
            , _y(value)
            , _z(value)
        {
        }

        constexpr float3(float xValue, float yValue, float zValue) noexcept
            : _x(xValue)
            , _y(yValue)
            , _z(zValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        const float& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        float3 operator+(const float3& other) const noexcept
        {
            return float3(_x + other._x, _y + other._y, _z + other._z);
        }

        float3 operator-(const float3& other) const noexcept
        {
            return float3(_x - other._x, _y - other._y, _z - other._z);
        }

        float3 operator*(const float3& other) const noexcept
        {
            return float3(_x * other._x, _y * other._y, _z * other._z);
        }

        float3 operator/(const float3& other) const noexcept
        {
            return float3(_x / other._x, _y / other._y, _z / other._z);
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
            return float3(-_x, -_y, -_z);
        }

        float dot(const float3& other) const noexcept
        {
            return _x * other._x + _y * other._y + _z * other._z;
        }

        float3 cross(const float3& other) const noexcept
        {
            return float3(
                _y * other._z - _z * other._y,
                _z * other._x - _x * other._z,
                _x * other._y - _y * other._x);
        }

        bool operator==(const float3& other) const noexcept
        {
            return _x == other._x && _y == other._y && _z == other._z;
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
        union
        {
            struct { float _x; float _y; float _z; float _w; };
            float _value[4];
        };

        constexpr float4() noexcept : _x(0.0f), _y(0.0f), _z(0.0f), _w(0.0f) {}
        constexpr explicit float4(float value) noexcept
            : _x(value)
            , _y(value)
            , _z(value)
            , _w(value)
        {
        }

        constexpr float4(float xValue, float yValue, float zValue, float wValue) noexcept
            : _x(xValue)
            , _y(yValue)
            , _z(zValue)
            , _w(wValue)
        {
        }

        constexpr float4(const float3& xyzValue, float wValue) noexcept
            : _x(xyzValue._x)
            , _y(xyzValue._y)
            , _z(xyzValue._z)
            , _w(wValue)
        {
        }

        float& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        const float& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        float4 operator+(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result._x, _mm_add_ps(_mm_load_ps(&_x), _mm_load_ps(&other._x)));
            return result;
#else
            return float4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
#endif
        }

        float4 operator-(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result._x, _mm_sub_ps(_mm_load_ps(&_x), _mm_load_ps(&other._x)));
            return result;
#else
            return float4(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
#endif
        }

        float4 operator*(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result._x, _mm_mul_ps(_mm_load_ps(&_x), _mm_load_ps(&other._x)));
            return result;
#else
            return float4(_x * other._x, _y * other._y, _z * other._z, _w * other._w);
#endif
        }

        float4 operator/(const float4& other) const noexcept
        {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
            float4 result;
            _mm_store_ps(&result._x, _mm_div_ps(_mm_load_ps(&_x), _mm_load_ps(&other._x)));
            return result;
#else
            return float4(_x / other._x, _y / other._y, _z / other._z, _w / other._w);
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
            const __m128 result = _mm_xor_ps(_mm_setr_ps(_x, _y, _z, _w), _mm_set1_ps(-0.0f));

            float values[4];
            _mm_storeu_ps(values, result);
            return float4(values[0], values[1], values[2], values[3]);
#else
            return float4(-_x, -_y, -_z, -_w);
#endif
        }

        bool operator==(const float4& other) const noexcept
        {
            return _x == other._x && _y == other._y && _z == other._z && _w == other._w;
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

    struct int2
    {
        union
        {
            struct { int32 _x; int32 _y; };
            int32 _value[2];
        };

        constexpr int2() noexcept : _x(0), _y(0) {}
        constexpr explicit int2(int32 value) noexcept : _x(value), _y(value) {}
        constexpr int2(int32 xValue, int32 yValue) noexcept : _x(xValue), _y(yValue) {}

        int32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        const int32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        bool operator==(const int2& other) const noexcept { return _x == other._x && _y == other._y; }
        bool operator!=(const int2& other) const noexcept { return !(*this == other); }
    };

    struct int3
    {
        union
        {
            struct { int32 _x; int32 _y; int32 _z; };
            int32 _value[3];
        };

        constexpr int3() noexcept : _x(0), _y(0), _z(0) {}
        constexpr explicit int3(int32 value) noexcept : _x(value), _y(value), _z(value) {}
        constexpr int3(int32 xValue, int32 yValue, int32 zValue) noexcept : _x(xValue), _y(yValue), _z(zValue) {}

        int32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        const int32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        bool operator==(const int3& other) const noexcept { return _x == other._x && _y == other._y && _z == other._z; }
        bool operator!=(const int3& other) const noexcept { return !(*this == other); }
    };

    struct int4
    {
        union
        {
            struct { int32 _x; int32 _y; int32 _z; int32 _w; };
            int32 _value[4];
        };

        constexpr int4() noexcept : _x(0), _y(0), _z(0), _w(0) {}
        constexpr explicit int4(int32 value) noexcept : _x(value), _y(value), _z(value), _w(value) {}
        constexpr int4(int32 xValue, int32 yValue, int32 zValue, int32 wValue) noexcept : _x(xValue), _y(yValue), _z(zValue), _w(wValue) {}

        int32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        const int32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        bool operator==(const int4& other) const noexcept { return _x == other._x && _y == other._y && _z == other._z && _w == other._w; }
        bool operator!=(const int4& other) const noexcept { return !(*this == other); }
    };

    struct uint2
    {
        union
        {
            struct { uint32 _x; uint32 _y; };
            uint32 _value[2];
        };

        constexpr uint2() noexcept : _x(0), _y(0) {}
        constexpr explicit uint2(uint32 value) noexcept : _x(value), _y(value) {}
        constexpr uint2(uint32 xValue, uint32 yValue) noexcept : _x(xValue), _y(yValue) {}

        uint32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        const uint32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 2, "Scalar vector index must be less than 2.");
            return _value[index];
        }

        bool operator==(const uint2& other) const noexcept { return _x == other._x && _y == other._y; }
        bool operator!=(const uint2& other) const noexcept { return !(*this == other); }
    };

    struct uint3
    {
        union
        {
            struct { uint32 _x; uint32 _y; uint32 _z; };
            uint32 _value[3];
        };

        constexpr uint3() noexcept : _x(0), _y(0), _z(0) {}
        constexpr explicit uint3(uint32 value) noexcept : _x(value), _y(value), _z(value) {}
        constexpr uint3(uint32 xValue, uint32 yValue, uint32 zValue) noexcept : _x(xValue), _y(yValue), _z(zValue) {}

        uint32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        const uint32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 3, "Scalar vector index must be less than 3.");
            return _value[index];
        }

        bool operator==(const uint3& other) const noexcept { return _x == other._x && _y == other._y && _z == other._z; }
        bool operator!=(const uint3& other) const noexcept { return !(*this == other); }
    };

    struct uint4
    {
        union
        {
            struct { uint32 _x; uint32 _y; uint32 _z; uint32 _w; };
            uint32 _value[4];
        };

        constexpr uint4() noexcept : _x(0), _y(0), _z(0), _w(0) {}
        constexpr explicit uint4(uint32 value) noexcept : _x(value), _y(value), _z(value), _w(value) {}
        constexpr uint4(uint32 xValue, uint32 yValue, uint32 zValue, uint32 wValue) noexcept : _x(xValue), _y(yValue), _z(zValue), _w(wValue) {}

        uint32& operator[](size_t index) noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        const uint32& operator[](size_t index) const noexcept
        {
            KEYH_ASSERT_DEV(index < 4, "Scalar vector index must be less than 4.");
            return _value[index];
        }

        bool operator==(const uint4& other) const noexcept { return _x == other._x && _y == other._y && _z == other._z && _w == other._w; }
        bool operator!=(const uint4& other) const noexcept { return !(*this == other); }
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

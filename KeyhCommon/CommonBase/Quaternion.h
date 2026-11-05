#pragma once

#include "MathUtil.h"
#include "ScalarVectorTypes.h"

namespace keyh
{
    struct alignas(16) Quaternion
    {
        union
        {
            struct { float _x; float _y; float _z; float _w; };
            float _value[4];
        };

        constexpr Quaternion() noexcept : _x(0.0f), _y(0.0f), _z(0.0f), _w(1.0f) {}
        constexpr Quaternion(float xValue, float yValue, float zValue, float wValue) noexcept
            : _x(xValue), _y(yValue), _z(zValue), _w(wValue) {}

        static const Quaternion Identity;

        static Quaternion RotationX(float radian) noexcept;
        static Quaternion RotationY(float radian) noexcept;
        static Quaternion RotationZ(float radian) noexcept;
        static Quaternion fromAxisAngle(const float3& axis, float radian) noexcept;
        static Quaternion fromEuler(const float3& radian) noexcept;

        float       lengthSquared() const noexcept;
        float       length() const noexcept;

        Quaternion normalized() const noexcept;
        Quaternion conjugate() const noexcept;
        Quaternion inverse() const noexcept;

        float3 rotate(const float3& value) const noexcept;
        float3 toEuler() const noexcept;

        Quaternion operator*(const Quaternion& other) const noexcept;
        bool operator==(const Quaternion& other) const noexcept;
        bool operator!=(const Quaternion& other) const noexcept;
    };
}

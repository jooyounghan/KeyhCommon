#include "CommonBasePch.h"
#include "Quaternion.h"

namespace keyh
{
	const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

    Quaternion Quaternion::fromAxisAngle(const float3& axis, float radian) noexcept
    {
        const float halfAngle = radian * 0.5f;
        float sine = sinf(halfAngle);

        const float axisLength = axis.length();
		if (MathUtil::isEqual(axisLength, 1.0f) == false)
		{
			KEYH_ASSERT_DEV(false, "Axis must be a unit vector.");
            const float inverseAxisLength = 1.0f / sqrtf(axisLength);
			sine *= inverseAxisLength;
        }

        return Quaternion(
            axis._x * sine
            , axis._y * sine
            , axis._z * sine
            , cosf(halfAngle)
        );
    }

    Quaternion Quaternion::fromEuler(const float3& radian) noexcept
    {
        const float halfX = radian._x * 0.5f;
        const float halfY = radian._y * 0.5f;
        const float halfZ = radian._z * 0.5f;
        const float sinX = sinf(halfX);
        const float cosX = cosf(halfX);
        const float sinY = sinf(halfY);
        const float cosY = cosf(halfY);
        const float sinZ = sinf(halfZ);
        const float cosZ = cosf(halfZ);
        return Quaternion(
            sinX * cosY * cosZ - cosX * sinY * sinZ,
            cosX * sinY * cosZ + sinX * cosY * sinZ,
            cosX * cosY * sinZ - sinX * sinY * cosZ,
            cosX * cosY * cosZ + sinX * sinY * sinZ).normalized();

        //return (
        //    Quaternion::RotationZ(radian._z) *
        //    Quaternion::RotationY(radian._y) *
        //    Quaternion::RotationX(radian._x)
        //    ).normalized();
    }

    float Quaternion::lengthSquared() const noexcept
    {
        return _x * _x + _y * _y + _z * _z + _w * _w;
    }

    float Quaternion::length() const noexcept
    {
        return sqrtf(lengthSquared());
    }

    Quaternion Quaternion::normalized() const noexcept
    {
        const float magnitude = length();
		if (magnitude <= 0.0f) 
        {
            KEYH_ASSERT_DEV(false, "Cannot normalize a zero quaternion.");
            return Identity;
        }

        const float inverseMagnitude = 1.0f / magnitude;
        return Quaternion(
            _x * inverseMagnitude
            , _y * inverseMagnitude
            , _z * inverseMagnitude
            , _w * inverseMagnitude
        );
    }

    Quaternion Quaternion::conjugate() const noexcept
    {
        return Quaternion(-_x, -_y, -_z, _w);
    }

    Quaternion Quaternion::inverse() const noexcept
    {
        const float magnitudeSquared = lengthSquared();
        if (magnitudeSquared <= 0.0f)
        {
            KEYH_ASSERT_DEV(false, "Cannot invert a zero quaternion.");
			return Identity;
        }

        const float reciprocal = 1.0f / magnitudeSquared;
        return Quaternion(
            -_x * reciprocal
            , -_y * reciprocal
            , -_z * reciprocal
            , _w * reciprocal
        );
    }

    float3 Quaternion::rotate(const float3& value) const noexcept
    {
        const Quaternion unit = normalized();
        const float3 vectorPart(unit._x, unit._y, unit._z);
        const float3 twiceCross = vectorPart.cross(value) * 2.0f;
        return value + twiceCross * unit._w + vectorPart.cross(twiceCross);
    }

    float3 Quaternion::toEuler() const noexcept
    {
        const Quaternion unit = normalized();
        const float sinXCosY = 2.0f * (unit._w * unit._x + unit._y * unit._z);
        const float cosXCosY = 1.0f - 2.0f * (unit._x * unit._x + unit._y * unit._y);
        const float sinY = 2.0f * (unit._w * unit._y - unit._z * unit._x);
        const float sinZCosY = 2.0f * (unit._w * unit._z + unit._x * unit._y);
        const float cosZCosY = 1.0f - 2.0f * (unit._y * unit._y + unit._z * unit._z);
        const float clampedSinY = MathUtil::clamp(sinY, -1.0f, 1.0f);
        return float3(atan2f(sinXCosY, cosXCosY), asinf(clampedSinY), atan2f(sinZCosY, cosZCosY));
    }

    Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
    {
        return Quaternion(
            _w * other._x + _x * other._w + _y * other._z - _z * other._y,
            _w * other._y - _x * other._z + _y * other._w + _z * other._x,
            _w * other._z + _x * other._y - _y * other._x + _z * other._w,
            _w * other._w - _x * other._x - _y * other._y - _z * other._z);
    }

    bool Quaternion::operator==(const Quaternion& other) const noexcept
    {
        return _x == other._x && _y == other._y && _z == other._z && _w == other._w;
    }

    bool Quaternion::operator!=(const Quaternion& other) const noexcept
    {
        return !(*this == other);
    }
}
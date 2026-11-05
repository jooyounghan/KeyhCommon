#include "CommonBasePch.h"
#include "ScalarVectorTypes.h"

namespace keyh
{
    float float3::lengthSquared() const noexcept
    {
        return _x * _x + _y * _y + _z * _z;
    }

    float float3::length() const noexcept
    {
        return sqrtf(lengthSquared());
    }

    float3 float3::sin() const noexcept
    {
        return float3(sinf(_x), sinf(_y), sinf(_z));
    }

    float3 float3::cos() const noexcept
    {
        return float3(cosf(_x), cosf(_y), cosf(_z));
    }

    float float4::lengthSquared() const noexcept
    {
        return _x * _x + _y * _y + _z * _z + _w * _w;
    }

    float float4::length() const noexcept
    {
        return sqrtf(lengthSquared());
    }

    float4 float4::sin() const noexcept
    {
        return float4(sinf(_x), sinf(_y), sinf(_z), sinf(_w));
    }

    float4 float4::cos() const noexcept
    {
        return float4(cosf(_x), cosf(_y), cosf(_z), cosf(_w));
    }
}

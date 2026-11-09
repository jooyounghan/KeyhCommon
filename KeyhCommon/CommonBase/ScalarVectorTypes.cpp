#include "CommonBasePch.h"
#include "ScalarVectorTypes.h"

namespace keyh
{
    half half::fromFloat(float value) noexcept
    {
        uint32 bits = 0;
        memcpy(&bits, &value, sizeof(bits));

        const uint16 sign = static_cast<uint16>((bits >> 16) & 0x8000u);
        const uint32 exponent = (bits >> 23) & 0xffu;
        const uint32 mantissa = bits & 0x7fffffu;
        half result{};

        if (exponent == 0xffu)
        {
            uint16 payload = static_cast<uint16>(mantissa >> 13);
            if (mantissa != 0)
                payload = static_cast<uint16>(payload | 0x0200u);
            result._bits = static_cast<uint16>(sign | 0x7c00u | payload);
        }
        else if (exponent >= 143u)
        {
            result._bits = static_cast<uint16>(sign | 0x7c00u);
        }
        else if (exponent <= 112u)
        {
            if (exponent < 102u)
            {
                result._bits = sign;
            }
            else
            {
                const uint32 significand = mantissa | 0x800000u;
                const uint32 shift = 126u - exponent;
                const uint32 rounded = significand >> shift;
                const uint32 remainder = significand & ((1u << shift) - 1u);
                const uint32 midpoint = 1u << (shift - 1u);
                result._bits = static_cast<uint16>(sign | (rounded +
                    (remainder > midpoint || (remainder == midpoint && (rounded & 1u) != 0u) ? 1u : 0u)));
            }
        }
        else
        {
            const uint32 rounded = mantissa + 0xfffu + ((mantissa >> 13) & 1u);
            result._bits = static_cast<uint16>(sign | (((exponent - 112u) << 10) + (rounded >> 13)));
        }
        return result;
    }

    float half::toFloat(half value) noexcept
    {
        const uint32 sign = static_cast<uint32>(value._bits & 0x8000u) << 16;
        const uint32 exponent = (value._bits >> 10) & 0x1fu;
        uint32 mantissa = value._bits & 0x03ffu;
        uint32 bits = 0;

        if (exponent == 0)
        {
            if (mantissa == 0)
            {
                bits = sign;
            }
            else
            {
                uint32 floatExponent = 113u;
                while ((mantissa & 0x0400u) == 0)
                {
                    mantissa <<= 1;
                    --floatExponent;
                }
                bits = sign | (floatExponent << 23) | ((mantissa & 0x03ffu) << 13);
            }
        }
        else
        {
            bits = sign | ((exponent == 0x1fu ? 0xffu : exponent + 112u) << 23) | (mantissa << 13);
        }

        float result = 0.0f;
        memcpy(&result, &bits, sizeof(result));
        return result;
    }

    half2::half2(const float2& value) noexcept : half2(value._x, value._y) {}

    float2 half2::toFloat2() const noexcept
    {
        return float2(half::toFloat(_x), half::toFloat(_y));
    }

    half4::half4(const float4& value) noexcept
        : half4(value._x, value._y, value._z, value._w) {}

    float4 half4::toFloat4() const noexcept
    {
        return float4(half::toFloat(_x), half::toFloat(_y), half::toFloat(_z), half::toFloat(_w));
    }

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

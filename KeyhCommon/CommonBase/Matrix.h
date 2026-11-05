#pragma once
#include "ScalarVectorTypes.h"

namespace keyh
{
    struct Quaternion;

    struct Matrix
    {
        union
        {
            struct 
            { 
                float4 _x;
                float4 _y;
                float4 _z;
                float4 _w; 
            };
            float _value[16];
        };

        Matrix() noexcept;
        explicit Matrix(float diagonal) noexcept;
        explicit Matrix(const float* values) noexcept;

        static const Matrix Identity;

        static Matrix translation(const float3& position) noexcept;
        static Matrix scaling(const float3& scale) noexcept;
        static Matrix rotation(const Quaternion& rotation) noexcept;
        static Matrix compose(const float3& position, const Quaternion& rotation, const float3& scale) noexcept;

        float& at(size_t row, size_t column) noexcept;
        const float& at(size_t row, size_t column) const noexcept;
        float* data() noexcept;
        const float* data() const noexcept;

        Matrix transposed() const noexcept;
        float determinant() const noexcept;
        bool tryInverse(Matrix& result) const noexcept;
        Matrix inverse() const noexcept;
        bool decompose(float3& position, Quaternion& rotation, float3& scale) const noexcept;

        float4 operator*(const float4& vector) const noexcept;
        Matrix operator+(const Matrix& other) const noexcept;
        Matrix operator-(const Matrix& other) const noexcept;
        Matrix operator*(const Matrix& other) const noexcept;
        Matrix operator*(float scalar) const noexcept;
        bool operator==(const Matrix& other) const noexcept;
        bool operator!=(const Matrix& other) const noexcept;
    };

    Matrix operator*(float scalar, const Matrix& matrix) noexcept;
}

#include "CommonBasePch.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "MathUtil.h"

namespace keyh
{
    const Matrix Matrix::Identity = Matrix(1.0f);

    Matrix::Matrix() noexcept
        : Matrix(1.0f)
    {
    }

    Matrix::Matrix(float diagonal) noexcept
        : _value{}
    {
        _value[0] = diagonal;
        _value[5] = diagonal;
        _value[10] = diagonal;
        _value[15] = diagonal;
    }

    Matrix::Matrix(const float* values) noexcept
        : _value{}
    {
        KEYH_ASSERT(values != nullptr, "Matrix source values must not be null.");
        if (values == nullptr) return;
        for (size_t index = 0; index < 16; ++index) _value[index] = values[index];
    }

    Matrix Matrix::translation(const float3& position) noexcept
    {
        Matrix result;
        result._value[12] = position._x;
        result._value[13] = position._y;
        result._value[14] = position._z;
        return result;
    }

    Matrix Matrix::scaling(const float3& scale) noexcept
    {
        Matrix result(0.0f);
        result._value[0] = scale._x;
        result._value[5] = scale._y;
        result._value[10] = scale._z;
        result._value[15] = 1.0f;
        return result;
    }

    Matrix Matrix::rotation(const Quaternion& inputRotation) noexcept
    {
        const Quaternion rotationValue = inputRotation.normalized();
        const float xx = rotationValue._x * rotationValue._x;
        const float yy = rotationValue._y * rotationValue._y;
        const float zz = rotationValue._z * rotationValue._z;
        const float xy = rotationValue._x * rotationValue._y;
        const float xz = rotationValue._x * rotationValue._z;
        const float yz = rotationValue._y * rotationValue._z;
        const float wx = rotationValue._w * rotationValue._x;
        const float wy = rotationValue._w * rotationValue._y;
        const float wz = rotationValue._w * rotationValue._z;

        Matrix result;
        result._value[0] = 1.0f - 2.0f * (yy + zz);
        result._value[1] = 2.0f * (xy + wz);
        result._value[2] = 2.0f * (xz - wy);
        result._value[4] = 2.0f * (xy - wz);
        result._value[5] = 1.0f - 2.0f * (xx + zz);
        result._value[6] = 2.0f * (yz + wx);
        result._value[8] = 2.0f * (xz + wy);
        result._value[9] = 2.0f * (yz - wx);
        result._value[10] = 1.0f - 2.0f * (xx + yy);
        return result;
    }

    Matrix Matrix::compose(const float3& position, const Quaternion& rotationValue, const float3& scale) noexcept
    {
        return translation(position) * rotation(rotationValue) * scaling(scale);
    }

    float& Matrix::at(size_t row, size_t column) noexcept
    {
        KEYH_ASSERT(row < 4 && column < 4, "Matrix row and column indices must be less than 4.");
        return _value[column * 4 + row];
    }

    const float& Matrix::at(size_t row, size_t column) const noexcept
    {
        KEYH_ASSERT(row < 4 && column < 4, "Matrix row and column indices must be less than 4.");
        return _value[column * 4 + row];
    }

    float* Matrix::data() noexcept { return _value; }
    const float* Matrix::data() const noexcept { return _value; }

    Matrix Matrix::transposed() const noexcept
    {
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
        Matrix result(0.0f);
        __m128 column0 = _mm_load_ps(_value);
        __m128 column1 = _mm_load_ps(_value + 4);
        __m128 column2 = _mm_load_ps(_value + 8);
        __m128 column3 = _mm_load_ps(_value + 12);
        _MM_TRANSPOSE4_PS(column0, column1, column2, column3);
        _mm_store_ps(result._value, column0);
        _mm_store_ps(result._value + 4, column1);
        _mm_store_ps(result._value + 8, column2);
        _mm_store_ps(result._value + 12, column3);
        return result;
#else
        Matrix result(0.0f);
        for (size_t row = 0; row < 4; ++row)
            for (size_t column = 0; column < 4; ++column)
                result.at(row, column) = at(column, row);
        return result;
#endif
    }

    float Matrix::determinant() const noexcept
    {
        const float a00 = at(0, 0), a01 = at(0, 1), a02 = at(0, 2), a03 = at(0, 3);
        const float a10 = at(1, 0), a11 = at(1, 1), a12 = at(1, 2), a13 = at(1, 3);
        const float a20 = at(2, 0), a21 = at(2, 1), a22 = at(2, 2), a23 = at(2, 3);
        const float a30 = at(3, 0), a31 = at(3, 1), a32 = at(3, 2), a33 = at(3, 3);

        const float minor0 = a22 * a33 - a23 * a32;
        const float minor1 = a21 * a33 - a23 * a31;
        const float minor2 = a21 * a32 - a22 * a31;
        const float minor3 = a20 * a33 - a23 * a30;
        const float minor4 = a20 * a32 - a22 * a30;
        const float minor5 = a20 * a31 - a21 * a30;

        return a00 * (a11 * minor0 - a12 * minor1 + a13 * minor2)
            - a01 * (a10 * minor0 - a12 * minor3 + a13 * minor4)
            + a02 * (a10 * minor1 - a11 * minor3 + a13 * minor5)
            - a03 * (a10 * minor2 - a11 * minor4 + a12 * minor5);
    }

    bool Matrix::tryInverse(Matrix& result) const noexcept
    {
        const float a00 = at(0, 0), a01 = at(0, 1), a02 = at(0, 2), a03 = at(0, 3);
        const float a10 = at(1, 0), a11 = at(1, 1), a12 = at(1, 2), a13 = at(1, 3);
        const float a20 = at(2, 0), a21 = at(2, 1), a22 = at(2, 2), a23 = at(2, 3);
        const float a30 = at(3, 0), a31 = at(3, 1), a32 = at(3, 2), a33 = at(3, 3);

        const float minor0 = a22 * a33 - a23 * a32;
        const float minor1 = a21 * a33 - a23 * a31;
        const float minor2 = a21 * a32 - a22 * a31;
        const float minor3 = a20 * a33 - a23 * a30;
        const float minor4 = a20 * a32 - a22 * a30;
        const float minor5 = a20 * a31 - a21 * a30;
        const float minor6 = a12 * a33 - a13 * a32;
        const float minor7 = a11 * a33 - a13 * a31;
        const float minor8 = a11 * a32 - a12 * a31;
        const float minor9 = a10 * a33 - a13 * a30;
        const float minor10 = a10 * a32 - a12 * a30;
        const float minor11 = a10 * a31 - a11 * a30;
        const float minor12 = a12 * a23 - a13 * a22;
        const float minor13 = a11 * a23 - a13 * a21;
        const float minor14 = a11 * a22 - a12 * a21;
        const float minor15 = a10 * a23 - a13 * a20;
        const float minor16 = a10 * a22 - a12 * a20;
        const float minor17 = a10 * a21 - a11 * a20;

        float cofactors[16];
        cofactors[0] =  a11 * minor0 - a12 * minor1 + a13 * minor2;
        cofactors[1] = -(a10 * minor0 - a12 * minor3 + a13 * minor4);
        cofactors[2] =  a10 * minor1 - a11 * minor3 + a13 * minor5;
        cofactors[3] = -(a10 * minor2 - a11 * minor4 + a12 * minor5);
        cofactors[4] = -(a01 * minor0 - a02 * minor1 + a03 * minor2);
        cofactors[5] =  a00 * minor0 - a02 * minor3 + a03 * minor4;
        cofactors[6] = -(a00 * minor1 - a01 * minor3 + a03 * minor5);
        cofactors[7] =  a00 * minor2 - a01 * minor4 + a02 * minor5;
        cofactors[8] =  a01 * minor6 - a02 * minor7 + a03 * minor8;
        cofactors[9] = -(a00 * minor6 - a02 * minor9 + a03 * minor10);
        cofactors[10] = a00 * minor7 - a01 * minor9 + a03 * minor11;
        cofactors[11] = -(a00 * minor8 - a01 * minor10 + a02 * minor11);
        cofactors[12] = -(a01 * minor12 - a02 * minor13 + a03 * minor14);
        cofactors[13] = a00 * minor12 - a02 * minor15 + a03 * minor16;
        cofactors[14] = -(a00 * minor13 - a01 * minor15 + a03 * minor17);
        cofactors[15] = a00 * minor14 - a01 * minor16 + a02 * minor17;

        const float determinantValue = a00 * cofactors[0] + a01 * cofactors[1] + a02 * cofactors[2] + a03 * cofactors[3];
        if (fabsf(determinantValue) <= 1.0e-8f) return false;

        const float inverseDeterminant = 1.0f / determinantValue;
        for (size_t row = 0; row < 4; ++row)
            for (size_t column = 0; column < 4; ++column)
                result.at(row, column) = cofactors[column * 4 + row] * inverseDeterminant;
        return true;
    }

    Matrix Matrix::inverse() const noexcept
    {
        Matrix result(0.0f);
        const bool invertible = tryInverse(result);
        KEYH_ASSERT_DEV(invertible, "Cannot invert a singular matrix.");
        return invertible ? result : Identity;
    }

    bool Matrix::decompose(float3& position, Quaternion& rotationValue, float3& scale) const noexcept
    {
        if (MathUtil::isGreater(fabsf(_value[3]), 0.0f) || MathUtil::isGreater(fabsf(_value[7]), 0.0f) ||
            MathUtil::isGreater(fabsf(_value[11]), 0.0f) || MathUtil::isGreater(fabsf(_value[15] - 1.0f), 0.0f)) return false;

        float3 right(_value[0], _value[1], _value[2]);
        float3 up(_value[4], _value[5], _value[6]);
        float3 forward(_value[8], _value[9], _value[10]);
        scale = float3(right.length(), up.length(), forward.length());
        if (MathUtil::isLessEqual(scale._x, 0.0f) || MathUtil::isLessEqual(scale._y, 0.0f) || MathUtil::isLessEqual(scale._z, 0.0f)) return false;
        right = right / scale._x;
        up = up / scale._y;
        forward = forward / scale._z;
        if (MathUtil::isGreater(fabsf(right.dot(up)), 0.0f) || MathUtil::isGreater(fabsf(right.dot(forward)), 0.0f) ||
            MathUtil::isGreater(fabsf(up.dot(forward)), 0.0f)) return false;

        const float handedness = right.dot(up.cross(forward));
        if (MathUtil::isLess(handedness, 0.0f))
        {
            scale._x = -scale._x;
            right = -right;
        }

        const float r00 = right._x, r01 = up._x, r02 = forward._x;
        const float r10 = right._y, r11 = up._y, r12 = forward._y;
        const float r20 = right._z, r21 = up._z, r22 = forward._z;
        Quaternion extracted;
        const float trace = r00 + r11 + r22;
        if (trace > 0.0f)
        {
            const float s = sqrtf(trace + 1.0f) * 2.0f;
            extracted = Quaternion((r21 - r12) / s, (r02 - r20) / s, (r10 - r01) / s, 0.25f * s);
        }
        else if (r00 > r11 && r00 > r22)
        {
            const float s = sqrtf(1.0f + r00 - r11 - r22) * 2.0f;
            extracted = Quaternion(0.25f * s, (r01 + r10) / s, (r02 + r20) / s, (r21 - r12) / s);
        }
        else if (r11 > r22)
        {
            const float s = sqrtf(1.0f + r11 - r00 - r22) * 2.0f;
            extracted = Quaternion((r01 + r10) / s, 0.25f * s, (r12 + r21) / s, (r02 - r20) / s);
        }
        else
        {
            const float s = sqrtf(1.0f + r22 - r00 - r11) * 2.0f;
            extracted = Quaternion((r02 + r20) / s, (r12 + r21) / s, 0.25f * s, (r10 - r01) / s);
        }
        position = float3(_value[12], _value[13], _value[14]);
        rotationValue = extracted.normalized();
        return true;
    }

    float4 Matrix::operator*(const float4& vector) const noexcept
    {
        return float4(
            _value[0] * vector._x + _value[4] * vector._y + _value[8] * vector._z + _value[12] * vector._w,
            _value[1] * vector._x + _value[5] * vector._y + _value[9] * vector._z + _value[13] * vector._w,
            _value[2] * vector._x + _value[6] * vector._y + _value[10] * vector._z + _value[14] * vector._w,
            _value[3] * vector._x + _value[7] * vector._y + _value[11] * vector._z + _value[15] * vector._w);
    }

    Matrix Matrix::operator+(const Matrix& other) const noexcept
    {
        Matrix result(0.0f);
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
        for (size_t column = 0; column < 4; ++column)
        {
            const __m128 left = _mm_load_ps(_value + column * 4);
            const __m128 right = _mm_load_ps(other._value + column * 4);
            _mm_store_ps(result._value + column * 4, _mm_add_ps(left, right));
        }
#else
        for (size_t index = 0; index < 16; ++index) result._value[index] = _value[index] + other._value[index];
#endif
        return result;
    }

    Matrix Matrix::operator-(const Matrix& other) const noexcept
    {
        Matrix result(0.0f);
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
        for (size_t column = 0; column < 4; ++column)
        {
            const __m128 left = _mm_load_ps(_value + column * 4);
            const __m128 right = _mm_load_ps(other._value + column * 4);
            _mm_store_ps(result._value + column * 4, _mm_sub_ps(left, right));
        }
#else
        for (size_t index = 0; index < 16; ++index) result._value[index] = _value[index] - other._value[index];
#endif
        return result;
    }

    Matrix Matrix::operator*(const Matrix& other) const noexcept
    {
        Matrix result(0.0f);
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
        for (size_t column = 0; column < 4; ++column)
        {
            const __m128 otherColumn0 = _mm_load_ps(other._value);
            const __m128 otherColumn1 = _mm_load_ps(other._value + 4);
            const __m128 otherColumn2 = _mm_load_ps(other._value + 8);
            const __m128 otherColumn3 = _mm_load_ps(other._value + 12);
            const __m128 resultColumn = _mm_add_ps(
                _mm_add_ps(
                    _mm_mul_ps(_mm_set1_ps(_value[column * 4]), otherColumn0),
                    _mm_mul_ps(_mm_set1_ps(_value[column * 4 + 1]), otherColumn1)),
                _mm_add_ps(
                    _mm_mul_ps(_mm_set1_ps(_value[column * 4 + 2]), otherColumn2),
                    _mm_mul_ps(_mm_set1_ps(_value[column * 4 + 3]), otherColumn3)));
            _mm_store_ps(result._value + column * 4, resultColumn);
        }
#else
        for (size_t row = 0; row < 4; ++row)
            for (size_t column = 0; column < 4; ++column)
                for (size_t inner = 0; inner < 4; ++inner)
                    result.at(row, column) += at(row, inner) * other.at(inner, column);
#endif
        return result;
    }

    Matrix Matrix::operator*(float scalar) const noexcept
    {
        Matrix result(0.0f);
#if KEYH_SCALAR_VECTOR_SIMD_ENABLED
        const __m128 scalarValue = _mm_set1_ps(scalar);
        for (size_t column = 0; column < 4; ++column)
        {
            const __m128 value = _mm_load_ps(_value + column * 4);
            _mm_store_ps(result._value + column * 4, _mm_mul_ps(value, scalarValue));
        }
#else
        for (size_t index = 0; index < 16; ++index) result._value[index] = _value[index] * scalar;
#endif
        return result;
    }

    bool Matrix::operator==(const Matrix& other) const noexcept
    {
        for (size_t index = 0; index < 16; ++index) if (_value[index] != other._value[index]) return false;
        return true;
    }

    bool Matrix::operator!=(const Matrix& other) const noexcept { return !(*this == other); }

    Matrix operator*(float scalar, const Matrix& matrix) noexcept { return matrix * scalar; }
}

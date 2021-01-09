// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Matrix4x4.h"

#include "CoreString.h"
#include "Numerics/Vector4.h"
#include "Numerics/Vector3.h"

using namespace std;

Matrix4x4::Matrix4x4()
{

}

Matrix4x4::Matrix4x4(
    float _11, float _12, float _13, float _14,
    float _21, float _22, float _23, float _24,
    float _31, float _32, float _33, float _34,
    float _41, float _42, float _43, float _44
)
{
    this->_11 = _11;
    this->_12 = _12;
    this->_13 = _13;
    this->_14 = _14;
    this->_21 = _21;
    this->_22 = _22;
    this->_23 = _23;
    this->_24 = _24;
    this->_31 = _31;
    this->_32 = _32;
    this->_33 = _33;
    this->_34 = _34;
    this->_41 = _41;
    this->_42 = _42;
    this->_43 = _43;
    this->_44 = _44;
}

Matrix4x4::Matrix4x4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
{
    _11 = v1.X;
    _12 = v1.Y;
    _13 = v1.Z;
    _14 = v1.W;

    _21 = v2.X;
    _22 = v2.Y;
    _23 = v2.Z;
    _24 = v2.W;

    _31 = v3.X;
    _32 = v3.Y;
    _33 = v3.Z;
    _34 = v3.W;

    _41 = v4.X;
    _42 = v4.Y;
    _43 = v4.Z;
    _44 = v4.W;
}

Matrix4x4::Matrix4x4(const Matrix4x4& copy)
{
    this->_11 = copy._11;
    this->_12 = copy._12;
    this->_13 = copy._13;
    this->_14 = copy._14;
    this->_21 = copy._21;
    this->_22 = copy._22;
    this->_23 = copy._23;
    this->_24 = copy._24;
    this->_31 = copy._31;
    this->_32 = copy._32;
    this->_33 = copy._33;
    this->_34 = copy._34;
    this->_41 = copy._41;
    this->_42 = copy._42;
    this->_43 = copy._43;
    this->_44 = copy._44;
}

bool Matrix4x4::Equals(const Matrix4x4& rh) const
{
    return *this == rh;
}

bool Matrix4x4::NearlyEquals(const Matrix4x4& rh, float epsilon) const
{
    return abs(_11 - rh._11) <= epsilon
        && abs(_12 - rh._12) <= epsilon
        && abs(_13 - rh._13) <= epsilon
        && abs(_14 - rh._14) <= epsilon
        && abs(_21 - rh._21) <= epsilon
        && abs(_22 - rh._22) <= epsilon
        && abs(_23 - rh._23) <= epsilon
        && abs(_24 - rh._24) <= epsilon
        && abs(_31 - rh._31) <= epsilon
        && abs(_32 - rh._32) <= epsilon
        && abs(_33 - rh._33) <= epsilon
        && abs(_34 - rh._34) <= epsilon
        && abs(_41 - rh._41) <= epsilon
        && abs(_42 - rh._42) <= epsilon
        && abs(_43 - rh._43) <= epsilon
        && abs(_44 - rh._44) <= epsilon;
}

size_t Matrix4x4::GetHashCode() const
{
    auto& rthis = (*this);
    return rthis[0].GetHashCode() ^ rthis[1].GetHashCode() ^ rthis[2].GetHashCode() ^ rthis[3].GetHashCode();
}

TRefPtr<String> Matrix4x4::ToString() const
{
    return String::Format("{{ {{{0}, {1}, {2}, {3}}} {{{4}, {5}, {6}, {7}}} {{{8}, {9}, {10}, {11}}} {{{12}, {13}, {14}, {15}}} }}",
        _11, _12, _13, _14,
        _21, _22, _23, _24,
        _31, _32, _33, _34,
        _41, _42, _43, _44
    );
}

Vector4 Matrix4x4::GetComponentOrDefault(size_t index) const
{
    if (!Contains(index, 0))
    {
        return Vector4{ };
    }

    const float* ptr = &_11;
    return *reinterpret_cast<const Vector4*>(ptr + 4 * index);
}

bool Matrix4x4::Contains(size_t index) const
{
    return index < Count;
}

bool Matrix4x4::Contains(size_t row, size_t column) const
{
    return row < Rows && column < Columns;
}

size_t Matrix4x4::Count_get() const
{
    return 16;
}

size_t Matrix4x4::Rows_get() const
{
    return 4;
}

size_t Matrix4x4::Columns_get() const
{
    return 4;
}

Matrix4x4& Matrix4x4::Transpose()
{
    swap(_12, _21);
    swap(_13, _31);
    swap(_14, _41);
    swap(_23, _32);
    swap(_24, _42);
    swap(_34, _43);

    return *this;
}

Matrix4x4& Matrix4x4::Invert()
{
    return *this = Inverse;
}

Vector3 Matrix4x4::TransformVector(const Vector3& vec) const
{
    return TransformVector(Vector4(vec.X, vec.Y, vec.Z, 0.0)).Cast<Vector3>();
}

Vector4 Matrix4x4::TransformVector(const Vector4& vec) const
{
    return Vector4(
        vec.X * _11 + vec.Y * _21 + vec.Z * _31 + vec.W * _41,
        vec.X * _12 + vec.Y * _22 + vec.Z * _32 + vec.W * _42,
        vec.X * _13 + vec.Y * _23 + vec.Z * _33 + vec.W * _43,
        vec.X * _14 + vec.Y * _24 + vec.Z * _34 + vec.W * _44
     );
}

bool Matrix4x4::IsIdentity_get() const
{
    return NearlyEquals(Identity, 0.0001f);
}

float Matrix4x4::Determinant_get() const
{
    return
        _14 * _23 * _32 * _41 - _13 * _24 * _32 * _41 -
        _14 * _22 * _33 * _41 + _12 * _24 * _33 * _41 +
        _13 * _22 * _34 * _41 - _12 * _23 * _34 * _41 -
        _14 * _23 * _31 * _42 + _13 * _24 * _31 * _42 +
        _14 * _21 * _33 * _42 - _11 * _24 * _33 * _42 -
        _13 * _21 * _34 * _42 + _11 * _23 * _34 * _42 +
        _14 * _22 * _31 * _43 - _12 * _24 * _31 * _43 -
        _14 * _21 * _32 * _43 + _11 * _24 * _32 * _43 +
        _12 * _21 * _34 * _43 - _11 * _22 * _34 * _43 -
        _13 * _22 * _31 * _44 + _12 * _23 * _31 * _44 +
        _13 * _21 * _32 * _44 - _11 * _23 * _32 * _44 -
        _12 * _21 * _33 * _44 + _11 * _22 * _33 * _44;
}

Matrix4x4 Matrix4x4::Inverse_get() const
{
    float A2323 = _33 * _44 - _34 * _43;
    float A1323 = _32 * _44 - _34 * _42;
    float A1223 = _32 * _43 - _33 * _42;
    float A0323 = _31 * _44 - _34 * _41;
    float A0223 = _31 * _43 - _33 * _41;
    float A0123 = _31 * _42 - _32 * _41;
    float A2313 = _23 * _44 - _24 * _43;
    float A1313 = _22 * _44 - _24 * _42;
    float A1213 = _22 * _43 - _23 * _42;
    float A2312 = _23 * _34 - _24 * _33;
    float A1312 = _22 * _34 - _24 * _32;
    float A1212 = _22 * _33 - _23 * _32;
    float A0313 = _21 * _44 - _24 * _41;
    float A0213 = _21 * _43 - _23 * _41;
    float A0312 = _21 * _34 - _24 * _31;
    float A0212 = _21 * _33 - _23 * _31;
    float A0113 = _21 * _42 - _22 * _41;
    float A0112 = _21 * _32 - _22 * _31;

    float det
        = _11 * (_22 * A2323 - _23 * A1323 + _24 * A1223)
        - _12 * (_21 * A2323 - _23 * A0323 + _24 * A0223)
        + _13 * (_21 * A1323 - _22 * A0323 + _24 * A0123)
        - _14 * (_21 * A1223 - _22 * A0223 + _23 * A0123);
    det = 1 / det;

    return Matrix4x4(
        det * (_22 * A2323 - _23 * A1323 + _24 * A1223),
        det * -(_12 * A2323 - _13 * A1323 + _14 * A1223),
        det * (_12 * A2313 - _13 * A1313 + _14 * A1213),
        det * -(_12 * A2312 - _13 * A1312 + _14 * A1212),
        det * -(_21 * A2323 - _23 * A0323 + _24 * A0223),
        det * (_11 * A2323 - _13 * A0323 + _14 * A0223),
        det * -(_11 * A2313 - _13 * A0313 + _14 * A0213),
        det * (_11 * A2312 - _13 * A0312 + _14 * A0212),
        det * (_21 * A1323 - _22 * A0323 + _24 * A0123),
        det * -(_11 * A1323 - _12 * A0323 + _14 * A0123),
        det * (_11 * A1313 - _12 * A0313 + _14 * A0113),
        det * -(_11 * A1312 - _12 * A0312 + _14 * A0112),
        det * -(_21 * A1223 - _22 * A0223 + _23 * A0123),
        det * (_11 * A1223 - _12 * A0223 + _13 * A0123),
        det * -(_11 * A1213 - _12 * A0213 + _13 * A0113),
        det * (_11 * A1212 - _12 * A0212 + _13 * A0112)
    );
}

Matrix4x4 Matrix4x4::Transposed_get() const
{
    return Matrix4x4(
        _11, _21, _31, _41,
        _12, _22, _32, _42,
        _13, _23, _33, _43,
        _14, _24, _34, _44
    );
}

const Vector4& Matrix4x4::operator [](size_t index) const
{
    const float* ptr = &_11;
    return *reinterpret_cast<const Vector4*>(ptr + 4 * index);
}

Vector4& Matrix4x4::operator [](size_t index)
{
    float* ptr = &_11;
    return *reinterpret_cast<Vector4*>(ptr + 4 * index);
}

Matrix4x4 Matrix4x4::operator -() const
{
    return Matrix4x4(
        -_11, -_21, -_31, -_41,
        -_12, -_22, -_32, -_42,
        -_13, -_23, -_33, -_43,
        -_14, -_24, -_34, -_44
    );
}

Matrix4x4 Matrix4x4::operator +(const Matrix4x4& right) const
{
    return Matrix4x4(
        _11 + right._11, _12 + right._12, _13 + right._13, _14 + right._14,
        _21 + right._21, _22 + right._22, _23 + right._23, _24 + right._24,
        _31 + right._31, _32 + right._32, _33 + right._33, _34 + right._34,
        _41 + right._41, _42 + right._42, _43 + right._43, _44 + right._44
    );
}

Matrix4x4 Matrix4x4::operator -(const Matrix4x4& right) const
{
    return Matrix4x4(
        _11 - right._11, _12 - right._12, _13 - right._13, _14 - right._14,
        _21 - right._21, _22 - right._22, _23 - right._23, _24 - right._24,
        _31 - right._31, _32 - right._32, _33 - right._33, _34 - right._34,
        _41 - right._41, _42 - right._42, _43 - right._43, _44 - right._44
    );
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4& right) const
{
    return Matrix4x4(
        _11 * right._11, _12 * right._12, _13 * right._13, _14 * right._14,
        _21 * right._21, _22 * right._22, _23 * right._23, _24 * right._24,
        _31 * right._31, _32 * right._32, _33 * right._33, _34 * right._34,
        _41 * right._41, _42 * right._42, _43 * right._43, _44 * right._44
    );
}

Matrix4x4 Matrix4x4::operator /(const Matrix4x4& right) const
{
    return Matrix4x4(
        _11 / right._11, _12 / right._12, _13 / right._13, _14 / right._14,
        _21 / right._21, _22 / right._22, _23 / right._23, _24 / right._24,
        _31 / right._31, _32 / right._32, _33 / right._33, _34 / right._34,
        _41 / right._41, _42 / right._42, _43 / right._43, _44 / right._44
    );
}

Matrix4x4& Matrix4x4::operator +=(const Matrix4x4& right)
{
    return *this = *this + right;
}

Matrix4x4& Matrix4x4::operator -=(const Matrix4x4& right)
{
    return *this = *this - right;
}

Matrix4x4& Matrix4x4::operator *=(const Matrix4x4& right)
{
    return *this = *this * right;
}

Matrix4x4& Matrix4x4::operator /=(const Matrix4x4& right)
{
    return *this = *this / right;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& A, const Matrix4x4& B)
{
    return Matrix4x4(
        A._11 * B._11 + A._12 * B._21 + A._13 * B._31 + A._14 * B._41,
        A._11 * B._12 + A._12 * B._22 + A._13 * B._32 + A._14 * B._42,
        A._11 * B._13 + A._12 * B._23 + A._13 * B._33 + A._14 * B._43,
        A._11 * B._14 + A._12 * B._24 + A._13 * B._34 + A._14 * B._44,

        A._21 * B._11 + A._22 * B._21 + A._23 * B._31 + A._24 * B._41,
        A._21 * B._12 + A._22 * B._22 + A._23 * B._32 + A._24 * B._42,
        A._21 * B._13 + A._22 * B._23 + A._23 * B._33 + A._24 * B._43,
        A._21 * B._14 + A._22 * B._24 + A._23 * B._34 + A._24 * B._44,

        A._31 * B._11 + A._32 * B._21 + A._33 * B._31 + A._34 * B._41,
        A._31 * B._12 + A._32 * B._22 + A._33 * B._32 + A._34 * B._42,
        A._31 * B._13 + A._32 * B._23 + A._33 * B._33 + A._34 * B._43,
        A._31 * B._14 + A._32 * B._24 + A._33 * B._34 + A._34 * B._44,

        A._41 * B._11 + A._42 * B._21 + A._43 * B._31 + A._44 * B._41,
        A._41 * B._12 + A._42 * B._22 + A._43 * B._32 + A._44 * B._42,
        A._41 * B._13 + A._42 * B._23 + A._43 * B._33 + A._44 * B._43,
        A._41 * B._14 + A._42 * B._24 + A._43 * B._34 + A._44 * B._44
    );
}

Matrix4x4 Matrix4x4::Identity = Matrix4x4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);
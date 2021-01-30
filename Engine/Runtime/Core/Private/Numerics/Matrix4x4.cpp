// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Matrix4x4.h"

#include "DXMathMinimal.h"
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
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMVECTOR V = XMVector3TransformCoord(XMLoadVector3(&vec), M);
    Vector3 v;
    XMStoreVector4(&v, V);
    return v;
}

Vector4 Matrix4x4::TransformVector(const Vector4& vec) const
{
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMVECTOR V = XMVector4Transform(XMLoadVector4(&vec), M);
    Vector4 v;
    XMStoreVector4(&v, V);
    return v;
}

bool Matrix4x4::IsIdentity_get() const
{
    return NearlyEquals(Identity, 0.0001f);
}

float Matrix4x4::Determinant_get() const
{
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMVECTOR V = XMMatrixDeterminant(M);
    return XMVectorGetX(V);
}

Matrix4x4 Matrix4x4::Inverse_get() const
{
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMVECTOR Det = XMMatrixDeterminant(M);
    XMMATRIX Inv = XMMatrixInverse(&Det, M);

    Matrix4x4 inv;
    XMStoreMatrix4x4(&inv, Inv);
    return inv;
}

Matrix4x4 Matrix4x4::Transposed_get() const
{
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMMATRIX T = XMMatrixTranspose(M);
    Matrix4x4 t;
    XMStoreMatrix4x4(&t, T);
    return t;
}

void Matrix4x4::BreakTransform(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const
{
    XMMATRIX M = XMLoadMatrix4x4(this);
    XMVECTOR T;
    XMVECTOR S;
    XMVECTOR Q;
    XMMatrixDecompose(&S, &Q, &T, M);

    XMStoreVector3(&outTranslation, T);
    XMStoreVector3(&outScale, S);
    XMStoreQuaternion(&outRotation, Q);
}

Vector3 Matrix4x4::Translation_get() const
{
    return Vector3(_41, _42, _43);
}

Vector3 Matrix4x4::Scale_get() const
{
    Vector3 t, s;
    Quaternion q;
    BreakTransform(t, s, q);
    return s;
}

Quaternion Matrix4x4::Rotation_get() const
{
    Vector3 t, s;
    Quaternion q;
    BreakTransform(t, s, q);
    return q;
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

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& a, const Matrix4x4& b)
{
    XMMATRIX A = XMLoadMatrix4x4(&a);
    XMMATRIX B = XMLoadMatrix4x4(&b);
    XMMATRIX M = XMMatrixMultiply(A, B);
    Matrix4x4 m;
    XMStoreMatrix4x4(&m, M);
    return m;
}

Matrix4x4 Matrix4x4::LookTo(const Vector3& location, const Vector3& dir, const Vector3& up)
{
    XMMATRIX M = XMMatrixLookToLH(XMLoadVector3(&location), XMLoadVector3(&dir), XMLoadVector3(&up));
    Matrix4x4 m;
    XMStoreMatrix4x4(&m, M);
    return m;
}

Matrix4x4 Matrix4x4::AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q)
{
    XMMATRIX M = XMMatrixAffineTransformation(XMLoadVector3(&s), XMVectorZero(), XMLoadQuaternion(&q), XMLoadVector3(&t));
    Matrix4x4 m;
    XMStoreMatrix4x4(&m, M);
    return m;
}

Matrix4x4 Matrix4x4::Identity = Matrix4x4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);
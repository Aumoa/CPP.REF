// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include "TRefPtr.h"
#include <compare>

namespace SC::Runtime::Core::Numerics
{
    struct Vector4;

#pragma pack(push, 8)
	struct CORE_API Matrix4x4
    {
        using This = Matrix4x4;

        double _11;
        double _12;
        double _13;
        double _14;
        double _21;
        double _22;
        double _23;
        double _24;
        double _31;
        double _32;
        double _33;
        double _34;
        double _41;
        double _42;
        double _43;
        double _44;

        Matrix4x4();
        Matrix4x4(double splat);
        Matrix4x4(double _11, double _12, double _13, double _14, double _21, double _22, double _23, double _24, double _31, double _32, double _33, double _34, double _41, double _42, double _43, double _44);
        Matrix4x4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
        Matrix4x4(const Matrix4x4& copy);

        bool Equals(const Matrix4x4& rh) const;
        bool NearlyEquals(const Matrix4x4& rh, double epsilon) const;
        size_t GetHashCode() const;
        TRefPtr<String> ToString() const;

        Vector4 GetComponentOrDefault(size_t index) const;
        template<TIsMatrixType T>
        inline void Construct(const T& matrix);
        template<TIsMatrixType T>
        inline T Cast() const;
        bool Contains(size_t index) const;
        bool Contains(size_t row, size_t column) const;

        vs_property_get(size_t, Count);
        size_t Count_get() const;
        vs_property_get(size_t, Rows);
        size_t Rows_get() const;
        vs_property_get(size_t, Columns);
        size_t Columns_get() const;

        Matrix4x4& Transpose();
        Matrix4x4& Invert();

        vs_property_get(bool, IsIdentity);
        bool IsIdentity_get() const;
        vs_property_get(double, Determinant);
        double Determinant_get() const;
        vs_property_get(Matrix4x4, Inverse);
        Matrix4x4 Inverse_get() const;
        vs_property_get(Matrix4x4, Transposed);
        Matrix4x4 Transposed_get() const;

        const Vector4& operator [](size_t index) const;
        Vector4& operator [](size_t index);

        Matrix4x4 operator -() const;
        Matrix4x4 operator +(const Matrix4x4& right) const;
        Matrix4x4 operator -(const Matrix4x4& right) const;
        Matrix4x4 operator *(const Matrix4x4& right) const;
        Matrix4x4 operator /(const Matrix4x4& right) const;

        bool operator ==(const Matrix4x4& right) const;
        bool operator !=(const Matrix4x4& right) const;

        Matrix4x4& operator +=(const Matrix4x4& right);
        Matrix4x4& operator -=(const Matrix4x4& right);
        Matrix4x4& operator *=(const Matrix4x4& right);
        Matrix4x4& operator /=(const Matrix4x4& right);

        static Matrix4x4 Identity;
	};
#pragma pack(pop)
}

#include "Matrix4x4.inl"
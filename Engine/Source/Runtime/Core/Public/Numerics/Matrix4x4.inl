// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsMatrixType T>
inline void Matrix4x4::Construct(const T& matrix)
{
    Vector4 v1 = matrix.GetComponentOrDefault(0);
    Vector4 v2 = matrix.GetComponentOrDefault(1);
    Vector4 v3 = matrix.GetComponentOrDefault(2);
    Vector4 v4 = matrix.GetComponentOrDefault(3);

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

template<TIsMatrixType T>
inline T Matrix4x4::Cast() const
{
	T value;
	value.Construct(*this);
	return value;
}
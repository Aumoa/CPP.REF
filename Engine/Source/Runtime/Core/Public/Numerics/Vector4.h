﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "TRefPtr.h"

struct Vector3;

#pragma pack(push, 4)
struct CORE_API Vector4 : public TNumericsBasicOperator<Vector4, float, float, float, float>
{
	using This = Vector4;

	float X;
	float Y;
	float Z;
	float W;

	Vector4();
	Vector4(const Vector3& v3, float w);
	Vector4(float x, float y, float z, float w);
	Vector4(float splat);
	Vector4(const Vector4& copy);

	bool Equals(const Vector4& rh) const;
	bool NearlyEquals(const Vector4& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	float GetComponentOrDefault(size_t index) const;
	template<TIsVectorType T>
	inline void Construct(const T& vector);
	template<TIsVectorType T>
	inline T Cast() const;
	bool Contains(size_t index) const;

	vs_property_get(size_t, Count);

	Vector4 GetClampedToMaxLength(float inMaxLength) const;

	vs_property_get(float, LengthSq);
	vs_property_get(float, Length);
	vs_property_get(Vector4, Normalized);
	vs_property(Vector4, Direction);

	const float& operator [](size_t index) const;
	float& operator [](size_t index);

	Vector4 operator -() const;
	Vector4 operator +(const Vector4& right) const;
	Vector4 operator -(const Vector4& right) const;
	Vector4 operator *(const Vector4& right) const;
	Vector4 operator /(const Vector4& right) const;
	float operator |(const Vector4& right) const;

	Vector4& operator +=(const Vector4& right);
	Vector4& operator -=(const Vector4& right);
	Vector4& operator *=(const Vector4& right);
	Vector4& operator /=(const Vector4& right);

	static float DistanceSq(const Vector4& left, const Vector4& right);
	static float Distance(const Vector4& left, const Vector4& right);
	static float DotProduct(const Vector4& left, const Vector4& right);
	static Vector4 Min(const Vector4& lh, const Vector4& rh);
	static Vector4 Max(const Vector4& lh, const Vector4& rh);

	static Vector4 Zero;
};
#pragma pack(pop)

CORE_API Vector4 operator +(float left, const Vector4& right);
CORE_API Vector4 operator -(float left, const Vector4& right);
CORE_API Vector4 operator *(float left, const Vector4& right);
CORE_API Vector4 operator /(float left, const Vector4& right);

#include "Vector4.inl"
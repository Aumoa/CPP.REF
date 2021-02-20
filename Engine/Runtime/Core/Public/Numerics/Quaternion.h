// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include <utility>
#include "TRefPtr.h"
#include "Mathematics/TAngleDef.h"

struct Vector3;
struct Vector4;
struct Matrix4x4;

#pragma pack(push, 4)
struct CORE_API Quaternion : public TNumericsBasicOperator<Quaternion, float, float, float, float>
{
	float X;
	float Y;
	float Z;
	float W;

	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3& xyz, float w);
	Quaternion(float splat);
	Quaternion(const Quaternion& copy);

	bool Equals(const Quaternion& rh) const;
	bool NearlyEquals(const Quaternion& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	float GetComponentOrDefault(size_t index) const;
	template<TIsVectorType T>
	inline void Construct(const T& vector);
	template<TIsVectorType T>
	inline T Cast() const;
	bool Contains(size_t index) const;

	vs_property_get(size_t, Count);
	size_t Count_get() const;

	std::pair<Vector3, float> ToAxisAngle() const;
	Vector3 RotateVector(const Vector3& v) const;
	Vector4 RotateVector(const Vector4& v) const;

	vs_property_get(float, LengthSq);
	float LengthSq_get() const;
	vs_property_get(float, Length);
	float Length_get() const;
	vs_property_get(Quaternion, Normalized);
	Quaternion Normalized_get() const;
	vs_property(Quaternion, Direction);
	Quaternion Direction_get() const;
	void Direction_set(const Quaternion& value);
	vs_property(Vector3, VectorPart);
	Vector3 VectorPart_get() const;
	void VectorPart_set(const Vector3& value);
	vs_property_get(float, Angle);
	float Angle_get() const;
	vs_property_get(Vector3, Axis);
	Vector3 Axis_get() const;
	vs_property_get(Quaternion, Inverse);
	Quaternion Inverse_get() const;
	vs_property_get(Matrix4x4, Matrix);
	Matrix4x4 Matrix_get() const;

	const float& operator [](size_t index) const;
	float& operator [](size_t index);

	Quaternion operator -() const;
	Quaternion operator +(const Quaternion& right) const;
	Quaternion operator -(const Quaternion& right) const;
	Quaternion operator *(const Quaternion& right) const;
	Quaternion operator /(const Quaternion& right) const;
	float operator |(const Quaternion& right) const;

	Quaternion& operator +=(const Quaternion& right);
	Quaternion& operator -=(const Quaternion& right);
	Quaternion& operator *=(const Quaternion& right);
	Quaternion& operator /=(const Quaternion& right);

	static float DotProduct(const Quaternion& left, const Quaternion& right);
	static Quaternion FromAxisAngle(const Vector3& axis, TDegrees<float> angle);
	static Quaternion Concatenate(const Quaternion& left, const Quaternion& right);
	static Quaternion FromMatrix(const Matrix4x4& rotationMatrix);
	static Quaternion FromEuler(TDegrees<float> yaw, TDegrees<float> pitch, TDegrees<float> roll);
	static Quaternion Lerp(const Quaternion& left, const Quaternion& right, float t);
	static Quaternion Slerp(const Quaternion& left, const Quaternion& right, float t);
	static Quaternion LookTo(const Vector3& forward, const Vector3& up);

	static Quaternion Identity;
};
#pragma pack(pop)

CORE_API Quaternion operator +(float left, const Quaternion& right);
CORE_API Quaternion operator -(float left, const Quaternion& right);
CORE_API Quaternion operator *(float left, const Quaternion& right);
CORE_API Quaternion operator /(float left, const Quaternion& right);

#include "Quaternion.inl"
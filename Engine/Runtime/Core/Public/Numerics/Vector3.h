// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "TRefPtr.h"

struct AxisAlignedCube;
struct Vector2;

#pragma pack(push, 4)
struct CORE_API Vector3 : public TNumericsBasicOperator<Vector3, float, float, float>
{
	using This = Vector3;

	class SelectControl
	{
		friend struct Vector3;

		char Value;

	public:
		inline constexpr SelectControl(bool v1, bool v2, bool v3);

		static constexpr SelectControl Less(const Vector3& lh, const Vector3& rh);
		static constexpr SelectControl Greater(const Vector3& lh, const Vector3& rh);
	};

	float X;
	float Y;
	float Z;

	Vector3();
	Vector3(const Vector2& v2, float z);
	Vector3(float x, float y, float z);
	Vector3(float splat);
	Vector3(const Vector3& copy);

	bool Equals(const Vector3& rh) const;
	bool NearlyEquals(const Vector3& rh, float epsilon) const;
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

	Vector3 GetClampedToMaxLength(float inMaxLength) const;

	vs_property_get(float, LengthSq);
	float LengthSq_get() const;
	vs_property_get(float, Length);
	float Length_get() const;
	vs_property_get(Vector3, Normalized);
	Vector3 Normalized_get() const;
	vs_property(Vector3, Direction);
	Vector3 Direction_get() const;
	void Direction_set(const Vector3& value);

	const float& operator [](size_t index) const;
	float& operator [](size_t index);

	Vector3 operator -() const;
	Vector3 operator +(const Vector3& right) const;
	Vector3 operator -(const Vector3& right) const;
	Vector3 operator *(const Vector3& right) const;
	Vector3 operator /(const Vector3& right) const;
	Vector3 operator ^(const Vector3& right) const;
	float operator |(const Vector3& right) const;

	Vector3& operator +=(const Vector3& right);
	Vector3& operator -=(const Vector3& right);
	Vector3& operator *=(const Vector3& right);
	Vector3& operator /=(const Vector3& right);
	Vector3& operator ^=(const Vector3& right);

	static float DistanceSq(const Vector3& left, const Vector3& right);
	static float Distance(const Vector3& left, const Vector3& right);
	static float DotProduct(const Vector3& left, const Vector3& right);
	static Vector3 CrossProduct(const Vector3& left, const Vector3& right);
	static Vector3 Min(const Vector3& lh, const Vector3& rh);
	static Vector3 Max(const Vector3& lh, const Vector3& rh);
	static Vector3 Select(const Vector3& lh, const Vector3& rh, const SelectControl& selectControl);

	static Vector3 Up;
	static Vector3 Forward;
	static Vector3 Right;
	static Vector3 AxisX;
	static Vector3 AxisY;
	static Vector3 AxisZ;
	static Vector3 Zero;
	static Vector3 One;
};
#pragma pack(pop)

CORE_API Vector3 operator +(float left, const Vector3& right);
CORE_API Vector3 operator -(float left, const Vector3& right);
CORE_API Vector3 operator *(float left, const Vector3& right);
CORE_API Vector3 operator /(float left, const Vector3& right);

#include "Vector3.inl"
// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include "TRefPtr.h"

class String;
struct Rect;

#pragma pack(push, 8)
struct CORE_API Vector2
{
	float X;
	float Y;

	Vector2();
	Vector2(float x, float y);
	Vector2(float splat);
	Vector2(const Vector2& copy);
		
	bool Equals(const Vector2& rh) const;
	bool NearlyEquals(const Vector2& rh, float epsilon) const;
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

	bool IsOverlap(const Rect& rect) const;

	vs_property_get(float, LengthSq);
	float LengthSq_get() const;
	vs_property_get(float, Length);
	float Length_get() const;
	vs_property_get(Vector2, Normalized);
	Vector2 Normalized_get() const;
	vs_property(Vector2, Direction);
	Vector2 Direction_get() const;
	void Direction_set(const Vector2& value);

	const float& operator [](size_t index) const;
	float& operator [](size_t index);

	Vector2 operator -() const;
	Vector2 operator +(const Vector2& right) const;
	Vector2 operator -(const Vector2& right) const;
	Vector2 operator *(const Vector2& right) const;
	Vector2 operator /(const Vector2& right) const;
	float operator |(const Vector2& right) const;

	bool operator ==(const Vector2& right) const;
	bool operator !=(const Vector2& right) const;
	bool operator < (const Vector2& right) const;
	bool operator <=(const Vector2& right) const;
	bool operator > (const Vector2& right) const;
	bool operator >=(const Vector2& right) const;
	std::weak_ordering operator <=>(const Vector2& right) const;

	Vector2& operator +=(const Vector2& right);
	Vector2& operator -=(const Vector2& right);
	Vector2& operator *=(const Vector2& right);
	Vector2& operator /=(const Vector2& right);

	static float DistanceSq(const Vector2& left, const Vector2& right);
	static float Distance(const Vector2& left, const Vector2& right);
	static float DotProduct(const Vector2& left, const Vector2& right);
};
#pragma pack(pop)

CORE_API Vector2 operator +(float left, const Vector2& right);
CORE_API Vector2 operator -(float left, const Vector2& right);
CORE_API Vector2 operator *(float left, const Vector2& right);
CORE_API Vector2 operator /(float left, const Vector2& right);

#include "Vector2.inl"
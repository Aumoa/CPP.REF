// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Vector3.h"

#pragma pack(push, 4)
struct CORE_API Plane : public TNumericsBasicOperator<Plane, Vector3, float>
{
	Vector3 Normal;
	float Distance;

public:
	Plane();
	Plane(const Vector3& inNormal, float inDistance);

	bool Equals(const Plane& rh) const;
	bool NearlyEquals(const Plane& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	float GetComponentOrDefault(size_t index) const;
	template<TIsVectorType T>
	inline void Construct(const T& vector);
	template<TIsVectorType T>
	inline T Cast() const;
	bool Contains(size_t index) const;

	vs_property_get_auto(size_t, Count, 4);
	vs_property_get(Plane, Normalized);
	Plane Normalized_get() const;

	const float& operator [](size_t index) const;
	float& operator [](size_t index);

	static float DotCoord(const Plane& inPlane, const Vector3& inVector);
};
#pragma pack(pop)

#include "Plane.inl"
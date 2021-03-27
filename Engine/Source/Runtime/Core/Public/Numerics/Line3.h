// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Vector3.h"

struct Ray3;

#pragma pack(push, 4)
struct CORE_API Line3 : public TNumericsBasicOperator<Line3, Vector3, Vector3>
{
	using This = Line3;

	Vector3 Start;
	Vector3 End;

	Line3();
	Line3(const Vector3& start, const Vector3& end);
	Line3(const Line3& copy);

	bool Equals(const Line3& rh) const;
	bool NearlyEquals(const Line3& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector3 GetPoint(float factor) const;

	vs_property_get(Vector3, Vector);

	static Line3 FromRay(const Ray3& ray);
};
#pragma pack(pop)
// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Vector3.h"

#pragma pack(push, 4)
struct CORE_API Sphere : public TNumericsBasicOperator<Sphere, Vector3, float>
{
	using This = Sphere;

	Vector3 Center;
	float Radius;

	Sphere();
	Sphere(const Vector3& center, float radius);
	Sphere(float cx, float cy, float cz, float radius);
	Sphere(const Sphere& copy);

	bool Equals(const Sphere& rh) const;
	bool NearlyEquals(const Sphere& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;
};
#pragma pack(pop)
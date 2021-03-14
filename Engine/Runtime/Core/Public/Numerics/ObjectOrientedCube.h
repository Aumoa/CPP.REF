// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include <vector>
#include "TRefPtr.h"
#include "Vector3.h"
#include "Quaternion.h"

#pragma pack(push, 4)
struct CORE_API ObjectOrientedCube : public TNumericsBasicOperator<ObjectOrientedCube, Vector3, Vector3, Quaternion>
{
	using This = ObjectOrientedCube;

	Vector3 Center;
	Vector3 Extent;
	Quaternion Rotation;

	ObjectOrientedCube();
	ObjectOrientedCube(const Vector3& center, const Vector3& extent, const Quaternion& rotation);
	ObjectOrientedCube(const ObjectOrientedCube& copy);

	bool Equals(const ObjectOrientedCube& rh) const;
	bool NearlyEquals(const ObjectOrientedCube& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	bool IsOverlap(const ObjectOrientedCube& cube) const;
	std::vector<Vector3> CalcVertices() const;

	vs_property_get(Vector3, AxisX);
	vs_property_get(Vector3, AxisY);
	vs_property_get(Vector3, AxisZ);
};
#pragma pack(pop)
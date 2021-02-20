// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include <optional>
#include "TRefPtr.h"
#include "Vector3.h"

struct Ray3;

#pragma pack(push, 4)
struct CORE_API AxisAlignedCube : public TNumericsBasicOperator<AxisAlignedCube, Vector3, Vector3>
{
	using This = AxisAlignedCube;

	Vector3 Min;
	Vector3 Max;

	AxisAlignedCube();
	AxisAlignedCube(const Vector3& min, const Vector3& max);
	AxisAlignedCube(const AxisAlignedCube& copy);

	bool Equals(const AxisAlignedCube& rh) const;
	bool NearlyEquals(const AxisAlignedCube& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector3 GetPoint(size_t index) const;

	vs_property_get(Vector3, Center);
	Vector3 Center_get() const;
};
#pragma pack(pop)
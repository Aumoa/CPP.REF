// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <optional>
#include "TRefPtr.h"
#include "Vector3.h"

struct Ray3;

#pragma pack(push, 8)
struct CORE_API AxisAlignedCube
{
	using This = AxisAlignedCube;

	Vector3 Center;
	Vector3 Extent;

	AxisAlignedCube();
	AxisAlignedCube(float x, float y, float z, float width, float height, float depth);
	AxisAlignedCube(const Vector3& center, const Vector3& extent);
	AxisAlignedCube(const AxisAlignedCube& copy);

	bool Equals(const AxisAlignedCube& rh) const;
	bool NearlyEquals(const AxisAlignedCube& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	bool IsOverlap(const Vector3& point) const;
	bool IsOverlap(const AxisAlignedCube& cube) const;
	bool IsOverlap(const Ray3& ray) const;
	std::optional<AxisAlignedCube> IsIntersect(const AxisAlignedCube& cube) const;
	std::optional<float> IsIntersect(const Ray3& ray) const;

	vs_property_get(float, Left);
	float Left_get() const;
	vs_property_get(float, Right);
	float Right_get() const;
	vs_property_get(float, Top);
	float Top_get() const;
	vs_property_get(float, Bottom);
	float Bottom_get() const;
	vs_property_get(float, Near);
	float Near_get() const;
	vs_property_get(float, Far);
	float Far_get() const;

	bool operator ==(const AxisAlignedCube& right) const;
	bool operator !=(const AxisAlignedCube& right) const;
	bool operator < (const AxisAlignedCube& right) const;
	bool operator <=(const AxisAlignedCube& right) const;
	bool operator > (const AxisAlignedCube& right) const;
	bool operator >=(const AxisAlignedCube& right) const;
	std::weak_ordering operator <=>(const AxisAlignedCube& right) const;
};
#pragma pack(pop)
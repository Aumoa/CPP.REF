// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <optional>
#include "Vector2.h"

#pragma pack(push, 8)
struct CORE_API Ray2
{
	Vector2 Origin;
	Vector2 Direction;
	std::optional<double> Distance;

	Ray2();
	Ray2(const Vector2& origin, const Vector2& direction, std::optional<double> distance = std::nullopt);
	Ray2(const Ray2& copy);

	bool Equals(const Ray2& rh) const;
	bool NearlyEquals(const Ray2& rh, double epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector2 GetPoint(double distance) const;
	bool IsOverlap(const Rect& rect) const;
	std::optional<double> IsIntersect(const Rect& rect) const;

	bool operator ==(const Ray2& rh) const;
	bool operator !=(const Ray2& rh) const;
	bool operator < (const Ray2& right) const;
	bool operator <=(const Ray2& right) const;
	bool operator > (const Ray2& right) const;
	bool operator >=(const Ray2& right) const;
	std::weak_ordering operator <=>(const Ray2& right) const;
};
#pragma pack(pop)
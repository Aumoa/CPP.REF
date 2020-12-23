// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <optional>
#include "Vector2.h"

namespace SC::Runtime::Core::Numerics
{
#pragma pack(push, 8)
	struct Ray2
	{
		Vector2 Origin;
		Vector2 Direction;
		std::optional<double> Distance;

		Ray2();
		Ray2(const Vector2& origin, const Vector2& direction, std::optional<double> distance = std::nullopt);

		bool Equals(const Ray2& rh) const;
		bool NearlyEquals(const Ray2& rh, double epsilon) const;
		size_t GetHashCode() const;
		TRefPtr<String> ToString() const;

		Vector2 GetPoint(double distance) const;
		bool IsOverlap(const Rectangle& rect) const;
		std::optional<double> IsIntersect(const Rectangle& rect) const;

		bool operator ==(const Ray2& rh) const;
		bool operator !=(const Ray2& rh) const;
		bool operator < (const Ray2& right) const;
		bool operator <=(const Ray2& right) const;
		bool operator > (const Ray2& right) const;
		bool operator >=(const Ray2& right) const;
		std::weak_ordering operator <=>(const Ray2& right) const;
	};
#pragma pack(pop)
}
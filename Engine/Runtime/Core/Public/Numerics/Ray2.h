// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include <optional>
#include "Vector2.h"

#pragma pack(push, 4)
struct CORE_API Ray2 : public TNumericsBasicOperator<Ray2, Vector2, Vector2, std::optional<float>>
{
	Vector2 Origin;
	Vector2 Direction;
	std::optional<float> Distance;

	Ray2();
	Ray2(const Vector2& origin, const Vector2& direction, std::optional<float> distance = std::nullopt);
	Ray2(const Ray2& copy);

	bool Equals(const Ray2& rh) const;
	bool NearlyEquals(const Ray2& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector2 GetPoint(float distance) const;
	Vector2 GetEndPoint() const;
};
#pragma pack(pop)
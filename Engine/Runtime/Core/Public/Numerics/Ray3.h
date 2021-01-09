// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include <optional>
#include "TRefPtr.h"
#include "Vector3.h"

#pragma pack(push, 4)
struct CORE_API Ray3 : public TNumericsBasicOperator<Ray3, Vector3, Vector3, std::optional<float>>
{
	Vector3 Origin;
	Vector3 Direction;
	std::optional<float> Distance;

	Ray3();
	Ray3(const Vector3& origin, const Vector3& direction, std::optional<float> distance = std::nullopt);
	Ray3(const Ray3& copy);

	bool Equals(const Ray3& rh) const;
	bool NearlyEquals(const Ray3& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector3 GetPoint(float distance) const;
	Vector3 GetEndPoint() const;
};
#pragma pack(pop)
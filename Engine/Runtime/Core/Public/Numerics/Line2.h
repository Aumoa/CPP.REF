// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Vector2.h"

struct Ray2;

#pragma pack(push, 4)
struct CORE_API Line2 : public TNumericsBasicOperator<Line2, Vector2, Vector2>
{
	using This = Line2;

	Vector2 Start;
	Vector2 End;

	Line2();
	Line2(const Vector2& start, const Vector2& end);
	Line2(const Line2& copy);

	bool Equals(const Line2& rh) const;
	bool NearlyEquals(const Line2& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector2 GetPoint(float factor) const;

	vs_property_get(Vector2, Vector);
	Vector2 Vector_get() const;

	static Line2 FromRay(const Ray2& ray);
};
#pragma pack(pop)
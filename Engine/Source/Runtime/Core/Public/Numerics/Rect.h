// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "TRefPtr.h"
#include "Vector2.h"

struct Ray2;

struct CORE_API Rect : public TNumericsBasicOperator<Rect, Vector2, Vector2>
{
	Vector2 Min;
	Vector2 Max;

	Rect();
	Rect(const Vector2& min, const Vector2& max);
	Rect(const Rect& copy);

	bool Equals(const Rect& rh) const;
	bool NearlyEquals(const Rect& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	vs_property_get(Vector2, Center);
	vs_property_get(float, Width);
	vs_property_get(float, Height);
	vs_property_get(float, Size);
};
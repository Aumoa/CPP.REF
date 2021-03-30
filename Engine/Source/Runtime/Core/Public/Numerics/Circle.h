// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Vector2.h"

#pragma pack(push, 4)
struct CORE_API Circle : public TNumericsBasicOperator<Circle, Vector2, float>
{
	using This = Circle;

	Vector2 Center;
	float Radius;

	Circle();
	Circle(const Vector2& center, float radius);
	Circle(float cx, float cy, float radius);
	Circle(const Circle& copy);

	bool Equals(const Circle& rh) const;
	bool NearlyEquals(const Circle& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;
};
#pragma pack(pop)
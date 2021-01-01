// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <optional>
#include "TRefPtr.h"

struct Vector2;
struct Ray2;

#pragma pack(push, 8)
struct CORE_API Rect
{
	float Left;
	float Top;
	float Right;
	float Bottom;

	Rect();
	Rect(float left, float top, float right, float bottom);
	Rect(const Vector2& lt, const Vector2& rb);
	Rect(const Rect& copy);

	bool Equals(const Rect& rh) const;
	bool NearlyEquals(const Rect& rh, float epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	bool IsOverlap(const Vector2& point) const;
	bool IsOverlap(const Rect& rect) const;
	bool IsOverlap(const Ray2& ray) const;
	std::optional<Rect> IsIntersect(const Rect& rect) const;
	std::optional<float> IsIntersect(const Ray2& ray) const;

	vs_property(Vector2, LeftTop);
	Vector2 LeftTop_get() const;
	void LeftTop_set(const Vector2& value);
	vs_property(Vector2, RightBottom);
	Vector2 RightBottom_get() const;
	void RightBottom_set(const Vector2& value);
	vs_property(float, Width);
	float Width_get() const;
	void Width_set(float value);
	vs_property(float, Height);
	float Height_get() const;
	void Height_set(float value);
	vs_property_get(float, Size);
	float Size_get() const;

	bool operator ==(const Rect& right) const;
	bool operator !=(const Rect& right) const;
	bool operator < (const Rect& right) const;
	bool operator <=(const Rect& right) const;
	bool operator > (const Rect& right) const;
	bool operator >=(const Rect& right) const;
	std::weak_ordering operator <=>(const Rect& right) const;
};
#pragma pack(pop)
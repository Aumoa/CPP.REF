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
struct CORE_API Rectangle
{
	double Left;
	double Top;
	double Right;
	double Bottom;

	Rectangle();
	Rectangle(double left, double top, double right, double bottom);
	Rectangle(const Vector2& lt, const Vector2& rb);
	Rectangle(const Rectangle& copy);

	bool Equals(const Rectangle& rh) const;
	bool NearlyEquals(const Rectangle& rh, double epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	bool IsOverlap(const Vector2& point) const;
	bool IsOverlap(const Rectangle& rect) const;
	bool IsOverlap(const Ray2& ray) const;
	std::optional<Rectangle> IsIntersect(const Rectangle& rect) const;
	std::optional<double> IsIntersect(const Ray2& ray) const;

	vs_property(Vector2, LeftTop);
	Vector2 LeftTop_get() const;
	void LeftTop_set(const Vector2& value);
	vs_property(Vector2, RightBottom);
	Vector2 RightBottom_get() const;
	void RightBottom_set(const Vector2& value);
	vs_property(double, Width);
	double Width_get() const;
	void Width_set(double value);
	vs_property(double, Height);
	double Height_get() const;
	void Height_set(double value);
	vs_property_get(double, Size);
	double Size_get() const;

	bool operator ==(const Rectangle& right) const;
	bool operator !=(const Rectangle& right) const;
	bool operator < (const Rectangle& right) const;
	bool operator <=(const Rectangle& right) const;
	bool operator > (const Rectangle& right) const;
	bool operator >=(const Rectangle& right) const;
	std::weak_ordering operator <=>(const Rectangle& right) const;
};
#pragma pack(pop)
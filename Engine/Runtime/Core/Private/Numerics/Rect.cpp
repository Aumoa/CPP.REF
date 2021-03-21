// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Rect.h"

#include "Numerics/Ray2.h"
#include "HashHelper.h"
#include "Core/String.h"
#include "IndexOutOfRangeException.h"

using namespace std;

Rect::Rect()
{

}

Rect::Rect(const Vector2& min, const Vector2& max)
{
	Min = min;
	Max = max;
}

Rect::Rect(const Rect& copy)
{
	Min = copy.Min;
	Max = copy.Max;
}

bool Rect::Equals(const Rect& rh) const
{
	return *this == rh;
}

bool Rect::NearlyEquals(const Rect& rh, float epsilon) const
{
	return Min.NearlyEquals(rh.Min, epsilon)
		&& Max.NearlyEquals(rh.Max, epsilon);
}

size_t Rect::GetHashCode() const
{
	return Min.GetHashCode() ^ Max.GetHashCode();
}

TRefPtr<String> Rect::ToString() const
{
	return String::Format(
		L"{{Min: {0}, Max: {1}, [{2} * {3}]}}",
		Min,
		Max,
		Width,
		Height
	);
}

Vector2 Rect::Center_get() const
{
	return (Min + Max) * 0.5f;
}

float Rect::Width_get() const
{
	return Max.X - Min.X;
}

float Rect::Height_get() const
{
	return Max.Y - Min.Y;
}

float Rect::Size_get() const
{
	return Width * Height;
}
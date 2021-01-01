// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Rect.h"

#include "Numerics/Vector2.h"
#include "Numerics/Ray2.h"
#include "HashHelper.h"
#include "CoreString.h"
#include "IndexOutOfRangeException.h"

using namespace std;

Rect::Rect()
{

}

Rect::Rect(float left, float top, float right, float bottom)
{
	Left = left;
	Top = top;
	Right = right;
	Bottom = bottom;
}

Rect::Rect(const Vector2& lt, const Vector2& rb)
{
	Left = lt.X;
	Top = lt.Y;
	Right = rb.X;
	Bottom = rb.Y;
}

Rect::Rect(const Rect& copy)
{
	Left = copy.Left;
	Top = copy.Top;
	Right = copy.Right;
	Bottom = copy.Bottom;
}

bool Rect::Equals(const Rect& rh) const
{
	return Left == rh.Left
		&& Top == rh.Top
		&& Right == rh.Right
		&& Bottom == rh.Bottom;
}

bool Rect::NearlyEquals(const Rect& rh, float epsilon) const
{
	return abs(rh.Left - Left) <= epsilon
		&& abs(rh.Top - Top) <= epsilon
		&& abs(rh.Right - Right) <= epsilon
		&& abs(rh.Bottom - Bottom) <= epsilon;
}

size_t Rect::GetHashCode() const
{
	return HashHelper::GetHashCode(Left)
		 ^ HashHelper::GetHashCode(Top)
		 ^ HashHelper::GetHashCode(Right)
		 ^ HashHelper::GetHashCode(Bottom);
}

TRefPtr<String> Rect::ToString() const
{
	return String::Format(
		L"{{LB: {0}, RB: {1}, [{2} * {3}]}}",
		LeftTop,
		RightBottom,
		Width,
		Height
	);
}

bool Rect::IsOverlap(const Vector2& point) const
{
	if (point.X >= Left && point.X <= Right &&
		point.Y >= Top && point.Y <= Bottom)
	{
		return true;
	}

	return false;
}

bool Rect::IsOverlap(const Rect& rect) const
{
	return !(Left > rect.Right ||
			 Right < rect.Left ||
			 Top > rect.Bottom ||
			 Bottom < rect.Top);
}

bool Rect::IsOverlap(const Ray2& ray) const
{
	return IsIntersect(ray).has_value();
}

optional<Rect> Rect::IsIntersect(const Rect& rect) const
{
	float left = max(Left, rect.Left);
	float top = max(Top, rect.Top);
	float right = min(Right, rect.Right);
	float bottom = min(Bottom, rect.Bottom);

	if (right >= left && bottom >= top)
	{
		return Rect(left, top, right, bottom);
	}
	else
	{
		return nullopt;
	}
}

optional<float> Rect::IsIntersect(const Ray2& ray) const
{
	Vector2 dirinv = 1.0 / ray.Direction;

	float t1 = (Left - ray.Origin.X) * dirinv.X;
	float t2 = (Right - ray.Origin.X) * dirinv.X;
	float t3 = (Bottom - ray.Origin.Y) * dirinv.Y;
	float t4 = (Top - ray.Origin.Y) * dirinv.Y;

	float tmin = max(min(t1, t2), min(t3, t4));
	float tmax = min(max(t1, t2), max(t3, t4));

	if (tmax < 0)
	{
		return nullopt;
	}

	if (tmin > tmax)
	{
		return nullopt;
	}

	return tmin;
}

Vector2 Rect::LeftTop_get() const
{
	return { Left, Top };
}

void Rect::LeftTop_set(const Vector2& value)
{
	Left = value.X;
	Top = value.Y;
}

Vector2 Rect::RightBottom_get() const
{
	return { Right, Bottom };
}

void Rect::RightBottom_set(const Vector2& value)
{
	Right = value.X;
	Bottom = value.Y;
}

float Rect::Width_get() const
{
	return Right - Left;
}

void Rect::Width_set(float value)
{
	Right = Left + value;
}

float Rect::Height_get() const
{
	return Bottom - Top;
}

void Rect::Height_set(float value)
{
	Bottom = Top + value;
}

float Rect::Size_get() const
{
	return Width * Height;
}

bool Rect::operator ==(const Rect& right) const
{
	return Left == right.Left && Top == right.Top && Right == right.Right && Bottom == right.Bottom;
}

bool Rect::operator !=(const Rect& right) const
{
	return Left != right.Left || Top != right.Top || Right != right.Right || Bottom != right.Bottom;
}

bool Rect::operator < (const Rect& right) const
{
	if (Left < right.Left)
	{
		return true;
	}
	else if (Left > right.Left)
	{
		return false;
	}
	else if (Top < right.Top)
	{
		return true;
	}
	else if (Top > right.Top)
	{
		return false;
	}
	else if (Right < right.Right)
	{
		return true;
	}
	else if (Right > right.Right)
	{
		return false;
	}
	else if (Bottom < right.Bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Rect::operator <=(const Rect& right) const
{
	if (Left < right.Left)
	{
		return true;
	}
	else if (Left > right.Left)
	{
		return false;
	}
	else if (Top < right.Top)
	{
		return true;
	}
	else if (Top > right.Top)
	{
		return false;
	}
	else if (Right < right.Right)
	{
		return true;
	}
	else if (Right > right.Right)
	{
		return false;
	}
	else if (Bottom <= right.Bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Rect::operator > (const Rect& right) const
{
	if (Left > right.Left)
	{
		return true;
	}
	else if (Left < right.Left)
	{
		return false;
	}
	else if (Top > right.Top)
	{
		return true;
	}
	else if (Top < right.Top)
	{
		return false;
	}
	else if (Right > right.Right)
	{
		return true;
	}
	else if (Right < right.Right)
	{
		return false;
	}
	else if (Bottom > right.Bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Rect::operator >=(const Rect& right) const
{
	if (Left > right.Left)
	{
		return true;
	}
	else if (Left < right.Left)
	{
		return false;
	}
	else if (Top > right.Top)
	{
		return true;
	}
	else if (Top < right.Top)
	{
		return false;
	}
	else if (Right > right.Right)
	{
		return true;
	}
	else if (Right < right.Right)
	{
		return false;
	}
	else if (Bottom >= right.Bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Rect::operator <=>(const Rect& right) const
{
	if (Left < right.Left)
	{
		return weak_ordering::less;
	}
	else if (Left > right.Left)
	{
		return weak_ordering::greater;
	}
	else if (Top < right.Top)
	{
		return weak_ordering::less;
	}
	else if (Top > right.Top)
	{
		return weak_ordering::greater;
	}
	else if (Right < right.Right)
	{
		return weak_ordering::less;
	}
	else if (Right > right.Right)
	{
		return weak_ordering::greater;
	}
	else if (Bottom < right.Bottom)
	{
		return weak_ordering::less;
	}
	else if (Bottom > right.Bottom)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}
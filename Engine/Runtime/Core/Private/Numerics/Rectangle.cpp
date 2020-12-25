// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Rectangle.h"

#include "Numerics/Vector2.h"
#include "Numerics/Ray2.h"
#include "HashHelper.h"
#include "CoreString.h"
#include "IndexOutOfRangeException.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace std;

Rectangle::Rectangle()
{

}

Rectangle::Rectangle(double left, double top, double right, double bottom)
{
	Left = left;
	Top = top;
	Right = right;
	Bottom = bottom;
}

Rectangle::Rectangle(const Vector2& lt, const Vector2& rb)
{
	Left = lt.X;
	Top = lt.Y;
	Right = rb.X;
	Bottom = rb.Y;
}

Rectangle::Rectangle(const Rectangle& copy)
{
	Left = copy.Left;
	Top = copy.Top;
	Right = copy.Right;
	Bottom = copy.Bottom;
}

bool Rectangle::Equals(const Rectangle& rh) const
{
	return Left == rh.Left
		&& Top == rh.Top
		&& Right == rh.Right
		&& Bottom == rh.Bottom;
}

bool Rectangle::NearlyEquals(const Rectangle& rh, double epsilon) const
{
	return abs(rh.Left - Left) <= epsilon
		&& abs(rh.Top - Top) <= epsilon
		&& abs(rh.Right - Right) <= epsilon
		&& abs(rh.Bottom - Bottom) <= epsilon;
}

size_t Rectangle::GetHashCode() const
{
	return HashHelper::GetHashCode(Left)
		 ^ HashHelper::GetHashCode(Top)
		 ^ HashHelper::GetHashCode(Right)
		 ^ HashHelper::GetHashCode(Bottom);
}

TRefPtr<String> Rectangle::ToString() const
{
	return String::Format(
		L"{{LB: {0}, RB: {1}, [{2} * {3}]}}",
		LeftTop,
		RightBottom,
		Width,
		Height
	);
}

bool Rectangle::IsOverlap(const Vector2& point) const
{
	if (point.X >= Left && point.X <= Right &&
		point.Y >= Top && point.Y <= Bottom)
	{
		return true;
	}

	return false;
}

bool Rectangle::IsOverlap(const Rectangle& rect) const
{
	return !(Left > rect.Right ||
			 Right < rect.Left ||
			 Top > rect.Bottom ||
			 Bottom < rect.Top);
}

bool Rectangle::IsOverlap(const Ray2& ray) const
{
	return IsIntersect(ray).has_value();
}

optional<Rectangle> Rectangle::IsIntersect(const Rectangle& rect) const
{
	double left = max(Left, rect.Left);
	double top = max(Top, rect.Top);
	double right = min(Right, rect.Right);
	double bottom = min(Bottom, rect.Bottom);

	if (right >= left && bottom >= top)
	{
		return Rectangle(left, top, right, bottom);
	}
	else
	{
		return nullopt;
	}
}

optional<double> Rectangle::IsIntersect(const Ray2& ray) const
{
	Vector2 dirinv = 1.0 / ray.Direction;

	double t1 = (Left - ray.Origin.X) * dirinv.X;
	double t2 = (Right - ray.Origin.X) * dirinv.X;
	double t3 = (Bottom - ray.Origin.Y) * dirinv.Y;
	double t4 = (Top - ray.Origin.Y) * dirinv.Y;

	double tmin = max(min(t1, t2), min(t3, t4));
	double tmax = min(max(t1, t2), max(t3, t4));

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

Vector2 Rectangle::LeftTop_get() const
{
	return { Left, Top };
}

void Rectangle::LeftTop_set(const Vector2& value)
{
	Left = value.X;
	Top = value.Y;
}

Vector2 Rectangle::RightBottom_get() const
{
	return { Right, Bottom };
}

void Rectangle::RightBottom_set(const Vector2& value)
{
	Right = value.X;
	Bottom = value.Y;
}

double Rectangle::Width_get() const
{
	return Right - Left;
}

void Rectangle::Width_set(double value)
{
	Right = Left + value;
}

double Rectangle::Height_get() const
{
	return Bottom - Top;
}

void Rectangle::Height_set(double value)
{
	Bottom = Top + value;
}

double Rectangle::Size_get() const
{
	return Width * Height;
}

bool Rectangle::operator ==(const Rectangle& right) const
{
	return Left == right.Left && Top == right.Top && Right == right.Right && Bottom == right.Bottom;
}

bool Rectangle::operator !=(const Rectangle& right) const
{
	return Left != right.Left || Top != right.Top || Right != right.Right || Bottom != right.Bottom;
}

bool Rectangle::operator < (const Rectangle& right) const
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

bool Rectangle::operator <=(const Rectangle& right) const
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

bool Rectangle::operator > (const Rectangle& right) const
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

bool Rectangle::operator >=(const Rectangle& right) const
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

weak_ordering Rectangle::operator <=>(const Rectangle& right) const
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
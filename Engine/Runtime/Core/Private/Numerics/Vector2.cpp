// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector2.h"

#include "IndexOutOfRangeException.h"
#include "Numerics/Rect.h"
#include "HashHelper.h"

using namespace std;

Vector2::Vector2()
{

}

Vector2::Vector2(double x, double y)
{
	X = x;
	Y = y;
}

Vector2::Vector2(double splat)
{
	X = splat;
	Y = splat;
}

Vector2::Vector2(const Vector2& copy)
{
	X = copy.X;
	Y = copy.Y;
}

bool Vector2::Equals(const Vector2& rh) const
{
	return (*this) == rh;
}

bool Vector2::NearlyEquals(const Vector2& rh, double epsilon) const
{
	return abs(X - rh.X) <= epsilon
		&& abs(Y - rh.Y) <= epsilon;
}

size_t Vector2::GetHashCode() const
{
	return HashHelper::GetHashCode(X)
		 ^ HashHelper::GetHashCode(Y);
}

TRefPtr<String> Vector2::ToString() const
{
	return String::Format(L"{{{0}, {1}}}", X, Y);
}

double Vector2::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const double* ptr = &X;
	return ptr[index];
}

bool Vector2::Contains(size_t index) const
{
	return index < Count;
}

size_t Vector2::Count_get() const
{
	return 2;
}

bool Vector2::IsOverlap(const Rect& rect) const
{
	if (X >= rect.Left && X <= rect.Right &&
		Y >= rect.Top && Y <= rect.Bottom)
	{
		return true;
	}
	return false;
}

double Vector2::LengthSq_get() const
{
	return X * X + Y * Y;
}

double Vector2::Length_get() const
{
	return sqrt(LengthSq);
}

Vector2 Vector2::Normalized_get() const
{
	return (*this) / Length;
}

Vector2 Vector2::Direction_get() const
{
	return Normalized;
}

void Vector2::Direction_set(const Vector2& value)
{
	(*this) = value * Length;
}

const double& Vector2::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const double* ptr = &X;
	return ptr[index];
}

double& Vector2::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	double* ptr = &X;
	return ptr[index];
}

Vector2 Vector2::operator -() const
{
	return Vector2(-X, -Y);
}

Vector2 Vector2::operator +(const Vector2& value) const
{
	return Vector2(X + value.X, Y + value.Y);
}

Vector2 Vector2::operator -(const Vector2& value) const
{
	return Vector2(X - value.X, Y - value.Y);
}

Vector2 Vector2::operator *(const Vector2& value) const
{
	return Vector2(X * value.X, Y * value.Y);
}

Vector2 Vector2::operator /(const Vector2& value) const
{
	return Vector2(X / value.X, Y / value.Y);
}

double Vector2::operator |(const Vector2& value) const
{
	return DotProduct(*this, value);
}

bool Vector2::operator ==(const Vector2& value) const
{
	return X == value.X && Y == value.Y;
}

bool Vector2::operator !=(const Vector2& value) const
{
	return X != value.X || Y != value.Y;
}

bool Vector2::operator < (const Vector2& right) const
{
	if (X < right.X)
	{
		return true;
	}
	else if (X > right.X)
	{
		return false;
	}
	else if (Y < right.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector2::operator <=(const Vector2& right) const
{
	if (X < right.X)
	{
		return true;
	}
	else if (X > right.X)
	{
		return false;
	}
	else if (Y <= right.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector2::operator > (const Vector2& right) const
{
	if (X > right.X)
	{
		return true;
	}
	else if (X < right.X)
	{
		return false;
	}
	else if (Y > right.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector2::operator >=(const Vector2& right) const
{
	if (X > right.X)
	{
		return true;
	}
	else if (X < right.X)
	{
		return false;
	}
	else if (Y >= right.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Vector2::operator <=>(const Vector2& right) const
{
	if (X < right.X)
	{
		return weak_ordering::less;
	}
	else if (X > right.X)
	{
		return weak_ordering::greater;
	}
	else if (Y < right.Y)
	{
		return weak_ordering::less;
	}
	else if (Y > right.Y)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}

Vector2& Vector2::operator +=(const Vector2& right)
{
	X += right.X;
	Y += right.Y;
	return *this;
}

Vector2& Vector2::operator -=(const Vector2& right)
{
	X -= right.X;
	Y -= right.Y;
	return *this;
}

Vector2& Vector2::operator *=(const Vector2& right)
{
	X *= right.X;
	Y *= right.Y;
	return *this;
}

Vector2& Vector2::operator /=(const Vector2& right)
{
	X /= right.X;
	Y /= right.Y;
	return *this;
}

double Vector2::DistanceSq(const Vector2& left, const Vector2& right)
{
	return (right - left).LengthSq;
}

double Vector2::Distance(const Vector2& left, const Vector2& right)
{
	return (right - left).Length;
}

double Vector2::DotProduct(const Vector2& left, const Vector2& right)
{
	return left.X * right.X + left.Y * right.Y;
}

Vector2 operator +(double left, const Vector2& right)
{
	return Vector2(left) + right;
}

Vector2 operator -(double left, const Vector2& right)
{
	return Vector2(left) - right;
}

Vector2 operator *(double left, const Vector2& right)
{
	return Vector2(left) * right;
}

Vector2 operator /(double left, const Vector2& right)
{
	return Vector2(left) / right;
}
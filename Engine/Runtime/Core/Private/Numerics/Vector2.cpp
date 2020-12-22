// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector2.h"

#include "IndexOutOfRangeException.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;

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
	constexpr size_t N = sizeof(Vector2) / sizeof(size_t);
	using TReint = const size_t(&)[N];
	TReint values = reinterpret_cast<TReint>(*this);
	size_t hash = values[0];
	for (size_t i = 1; i < N; ++i)
	{
		hash ^= values[i];
	}
	return hash;
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
	return Count >= index;
}

size_t Vector2::Count_get() const
{
	return 2;
}

bool Vector2::IsOverlap(const Rectangle& rect) const
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

Vector2& Vector2::operator +=(const Vector2& right)
{
	X += right.X;
	Y += right.Y;
	return *this;
}

Vector2& Vector2::operator -=(const Vector2& right)
{
	X -= right.X;
	Y -= right.X;
	return *this;
}

Vector2& Vector2::operator *=(const Vector2& right)
{
	X *= right.X;
	Y *= right.X;
	return *this;
}

Vector2& Vector2::operator /=(const Vector2& right)
{
	X /= right.X;
	Y /= right.X;
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

Vector2 Numerics::operator +(double left, const Vector2& right)
{
	return Vector2(left) + right;
}

Vector2 Numerics::operator -(double left, const Vector2& right)
{
	return Vector2(left) - right;
}

Vector2 Numerics::operator *(double left, const Vector2& right)
{
	return Vector2(left) * right;
}

Vector2 Numerics::operator /(double left, const Vector2& right)
{
	return Vector2(left) / right;
}
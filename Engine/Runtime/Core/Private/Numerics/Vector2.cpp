// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector2.h"

#include "IndexOutOfRangeException.h"
#include "Numerics/Rect.h"
#include "HashHelper.h"
#include "Mathematics/Math.h"

using namespace std;

Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1.0f, 1.0f);

Vector2::Vector2()
{

}

Vector2::Vector2(float x, float y)
{
	X = x;
	Y = y;
}

Vector2::Vector2(float splat)
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

bool Vector2::NearlyEquals(const Vector2& rh, float epsilon) const
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

float Vector2::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const float* ptr = &X;
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

float Vector2::LengthSq_get() const
{
	return X * X + Y * Y;
}

float Vector2::Length_get() const
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

const float& Vector2::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const float* ptr = &X;
	return ptr[index];
}

float& Vector2::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	float* ptr = &X;
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

float Vector2::operator |(const Vector2& value) const
{
	return DotProduct(*this, value);
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

float Vector2::DistanceSq(const Vector2& left, const Vector2& right)
{
	return (right - left).LengthSq;
}

float Vector2::Distance(const Vector2& left, const Vector2& right)
{
	return (right - left).Length;
}

float Vector2::DotProduct(const Vector2& left, const Vector2& right)
{
	return left.X * right.X + left.Y * right.Y;
}

Vector2 Vector2::Min(const Vector2& lh, const Vector2& rh)
{
	return { Math::Min(lh.X, rh.X), Math::Min(lh.Y, rh.Y) };
}

Vector2 Vector2::Max(const Vector2& lh, const Vector2& rh)
{
	return { Math::Max(lh.X, rh.X), Math::Max(lh.Y, rh.Y) };
}

Vector2 operator +(float left, const Vector2& right)
{
	return Vector2(left) + right;
}

Vector2 operator -(float left, const Vector2& right)
{
	return Vector2(left) - right;
}

Vector2 operator *(float left, const Vector2& right)
{
	return Vector2(left) * right;
}

Vector2 operator /(float left, const Vector2& right)
{
	return Vector2(left) / right;
}
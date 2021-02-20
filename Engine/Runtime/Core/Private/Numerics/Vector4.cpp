// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Vector4.h"

#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Vector3.h"
#include "Mathematics/Math.h"

using namespace std;

Vector4::Vector4()
{

}

Vector4::Vector4(const Vector3& v3, float w) : This(v3.X, v3.Y, v3.Z, w)
{

}

Vector4::Vector4(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Vector4::Vector4(float splat)
{
	X = splat;
	Y = splat;
	Z = splat;
	W = splat;
}

Vector4::Vector4(const Vector4& copy)
{
	X = copy.X;
	Y = copy.Y;
	Z = copy.Z;
	W = copy.W;
}

bool Vector4::Equals(const Vector4& rh) const
{
	return (*this) == rh;
}

bool Vector4::NearlyEquals(const Vector4& rh, float epsilon) const
{
	return abs(X - rh.X) <= epsilon
		&& abs(Y - rh.Y) <= epsilon
		&& abs(Z - rh.Z) <= epsilon
		&& abs(W - rh.W) <= epsilon;
}

size_t Vector4::GetHashCode() const
{
	return HashHelper::GetHashCode(X)
		 ^ HashHelper::GetHashCode(Y)
		 ^ HashHelper::GetHashCode(Z)
		 ^ HashHelper::GetHashCode(W);
}

TRefPtr<String> Vector4::ToString() const
{
	return String::Format(L"{{{0}, {1}, {2}, {3}}}", X, Y, Z, W);
}

float Vector4::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const float* ptr = &X;
	return ptr[index];
}

bool Vector4::Contains(size_t index) const
{
	return index < Count;
}

size_t Vector4::Count_get() const
{
	return 4;
}

Vector4 Vector4::GetClampedToMaxLength(float inMaxLength) const
{
	if (inMaxLength < Math::SmallNumber<>)
	{
		return Vector4::Zero;
	}

	const float VSq = LengthSq;
	// If vector length is over than max length,
	// clamp it to max length.
	if (VSq > Math::Square(inMaxLength))
	{
		float scale = inMaxLength * Math::InvSqrt(VSq);
		return *this * scale;
	}
	else
	{
		return *this;
	}
}

float Vector4::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

float Vector4::Length_get() const
{
	return sqrt(LengthSq);
}

Vector4 Vector4::Normalized_get() const
{
	return (*this) / Length;
}

Vector4 Vector4::Direction_get() const
{
	return Normalized;
}

void Vector4::Direction_set(const Vector4& value)
{
	(*this) = value * Length;
}

const float& Vector4::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const float* ptr = &X;
	return ptr[index];
}

float& Vector4::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	float* ptr = &X;
	return ptr[index];
}

Vector4 Vector4::operator -() const
{
	return Vector4(-X, -Y, -Z, -W);
}

Vector4 Vector4::operator +(const Vector4& value) const
{
	return Vector4(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
}

Vector4 Vector4::operator -(const Vector4& value) const
{
	return Vector4(X - value.X, Y - value.Y, Z - value.Z, W - value.W);
}

Vector4 Vector4::operator *(const Vector4& value) const
{
	return Vector4(X * value.X, Y * value.Y, Z * value.Z, W * value.W);
}

Vector4 Vector4::operator /(const Vector4& value) const
{
	return Vector4(X / value.X, Y / value.Y, Z / value.Z, W / value.W);
}

float Vector4::operator |(const Vector4& value) const
{
	return DotProduct(*this, value);
}

Vector4& Vector4::operator +=(const Vector4& right)
{
	X += right.X;
	Y += right.Y;
	Z += right.Z;
	W += right.W;
	return *this;
}

Vector4& Vector4::operator -=(const Vector4& right)
{
	X -= right.X;
	Y -= right.Y;
	Z -= right.Z;
	W -= right.W;
	return *this;
}

Vector4& Vector4::operator *=(const Vector4& right)
{
	X *= right.X;
	Y *= right.Y;
	Z *= right.Z;
	W *= right.W;
	return *this;
}

Vector4& Vector4::operator /=(const Vector4& right)
{
	X /= right.X;
	Y /= right.Y;
	Z /= right.Z;
	W /= right.W;
	return *this;
}

float Vector4::DistanceSq(const Vector4& left, const Vector4& right)
{
	return (right - left).LengthSq;
}

float Vector4::Distance(const Vector4& left, const Vector4& right)
{
	return (right - left).Length;
}

float Vector4::DotProduct(const Vector4& left, const Vector4& right)
{
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
}

Vector4 Vector4::Min(const Vector4& lh, const Vector4& rh)
{
	return { Math::Min(lh.X, rh.X), Math::Min(lh.Y, rh.Y), Math::Min(lh.Z, rh.Z), Math::Min(lh.W, rh.W) };
}

Vector4 Vector4::Max(const Vector4& lh, const Vector4& rh)
{
	return { Math::Max(lh.X, rh.X), Math::Max(lh.Y, rh.Y), Math::Max(lh.Z, rh.Z), Math::Max(lh.W, rh.W) };
}

Vector4 operator +(float left, const Vector4& right)
{
	return Vector4(left) + right;
}

Vector4 operator -(float left, const Vector4& right)
{
	return Vector4(left) - right;
}

Vector4 operator *(float left, const Vector4& right)
{
	return Vector4(left) * right;
}

Vector4 operator /(float left, const Vector4& right)
{
	return Vector4(left) / right;
}

Vector4 Vector4::Zero = Vector4(0, 0, 0, 0);
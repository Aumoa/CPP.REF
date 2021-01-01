// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector3.h"

#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Vector2.h"

using namespace std;

Vector3 Vector3::Up = Vector3(0, 0, 1.0f);
Vector3 Vector3::Right = Vector3(1.0f, 0, 0);
Vector3 Vector3::Forward = Vector3(0, 1.0f, 0);
Vector3 Vector3::AxisX = Vector3(1.0f, 0, 0);
Vector3 Vector3::AxisY = Vector3(0, 1.0f, 0);
Vector3 Vector3::AxisZ = Vector3(0, 0, 1.0f);
Vector3 Vector3::Zero = Vector3(0, 0, 0);
Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);

Vector3::Vector3()
{

}

Vector3::Vector3(const Vector2& v2, float z) : This(v2.X, v2.Y, z)
{

}

Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3::Vector3(float splat)
{
	X = splat;
	Y = splat;
	Z = splat;
}

Vector3::Vector3(const Vector3& copy)
{
	X = copy.X;
	Y = copy.Y;
	Z = copy.Z;
}

bool Vector3::Equals(const Vector3& rh) const
{
	return (*this) == rh;
}

bool Vector3::NearlyEquals(const Vector3& rh, float epsilon) const
{
	return abs(X - rh.X) <= epsilon
		&& abs(Y - rh.Y) <= epsilon
		&& abs(Z - rh.Z) <= epsilon;
}

size_t Vector3::GetHashCode() const
{
	return HashHelper::GetHashCode(X)
		 ^ HashHelper::GetHashCode(Y)
		 ^ HashHelper::GetHashCode(Z);
}

TRefPtr<String> Vector3::ToString() const
{
	return String::Format(L"{{{0}, {1}, {2}}}", X, Y, Z);
}

float Vector3::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const float* ptr = &X;
	return ptr[index];
}

bool Vector3::Contains(size_t index) const
{
	return index < Count;
}

size_t Vector3::Count_get() const
{
	return 3;
}

bool Vector3::IsOverlap(const AxisAlignedCube& cube) const
{
	return cube.IsOverlap(*this);
}

float Vector3::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z;
}

float Vector3::Length_get() const
{
	return sqrt(LengthSq);
}

Vector3 Vector3::Normalized_get() const
{
	return (*this) / Length;
}

Vector3 Vector3::Direction_get() const
{
	return Normalized;
}

void Vector3::Direction_set(const Vector3& value)
{
	(*this) = value * Length;
}

const float& Vector3::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const float* ptr = &X;
	return ptr[index];
}

float& Vector3::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	float* ptr = &X;
	return ptr[index];
}

Vector3 Vector3::operator -() const
{
	return Vector3(-X, -Y, -Z);
}

Vector3 Vector3::operator +(const Vector3& value) const
{
	return Vector3(X + value.X, Y + value.Y, Z + value.Z);
}

Vector3 Vector3::operator -(const Vector3& value) const
{
	return Vector3(X - value.X, Y - value.Y, Z - value.Z);
}

Vector3 Vector3::operator *(const Vector3& value) const
{
	return Vector3(X * value.X, Y * value.Y, Z * value.Z);
}

Vector3 Vector3::operator /(const Vector3& value) const
{
	return Vector3(X / value.X, Y / value.Y, Z / value.Z);
}

Vector3 Vector3::operator ^(const Vector3& value) const
{
	return CrossProduct(*this, value);
}

float Vector3::operator |(const Vector3& value) const
{
	return DotProduct(*this, value);
}

bool Vector3::operator ==(const Vector3& value) const
{
	return X == value.X && Y == value.Y && Z == value.Z;
}

bool Vector3::operator !=(const Vector3& value) const
{
	return X != value.X || Y != value.Y || Z != value.Z;
}

bool Vector3::operator < (const Vector3& right) const
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
	else if (Y > right.Y)
	{
		return false;
	}
	else if (Z < right.Z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector3::operator <=(const Vector3& right) const
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
	else if (Y > right.Y)
	{
		return false;
	}
	else if (Z <= right.Z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector3::operator > (const Vector3& right) const
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
	else if (Y < right.Y)
	{
		return false;
	}
	else if (Z > right.Z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector3::operator >=(const Vector3& right) const
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
	else if (Y < right.Y)
	{
		return false;
	}
	else if (Z >= right.Z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Vector3::operator <=>(const Vector3& right) const
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
	else if (Z < right.Z)
	{
		return weak_ordering::less;
	}
	else if (Z > right.Z)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}

Vector3& Vector3::operator +=(const Vector3& right)
{
	X += right.X;
	Y += right.Y;
	Z += right.Z;
	return *this;
}

Vector3& Vector3::operator -=(const Vector3& right)
{
	X -= right.X;
	Y -= right.Y;
	Z -= right.Z;
	return *this;
}

Vector3& Vector3::operator *=(const Vector3& right)
{
	X *= right.X;
	Y *= right.Y;
	Z *= right.Z;
	return *this;
}

Vector3& Vector3::operator /=(const Vector3& right)
{
	X /= right.X;
	Y /= right.Y;
	Z /= right.Z;
	return *this;
}

Vector3& Vector3::operator ^=(const Vector3& right)
{
	return (*this) = (*this) ^ right;
}

float Vector3::DistanceSq(const Vector3& left, const Vector3& right)
{
	return (right - left).LengthSq;
}

float Vector3::Distance(const Vector3& left, const Vector3& right)
{
	return (right - left).Length;
}

float Vector3::DotProduct(const Vector3& left, const Vector3& right)
{
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
}

Vector3 Vector3::CrossProduct(const Vector3& left, const Vector3& right)
{
	return Vector3(
		left.Y * right.Z - left.Z * right.Y,
		left.Z * right.X - left.X * right.Z,
		left.X * right.Y - left.Y * right.X
	);
}

Vector3 operator +(float left, const Vector3& right)
{
	return Vector3(left) + right;
}

Vector3 operator -(float left, const Vector3& right)
{
	return Vector3(left) - right;
}

Vector3 operator *(float left, const Vector3& right)
{
	return Vector3(left) * right;
}

Vector3 operator /(float left, const Vector3& right)
{
	return Vector3(left) / right;
}
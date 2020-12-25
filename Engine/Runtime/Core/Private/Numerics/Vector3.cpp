// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector3.h"

#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "Numerics/AxisAlignedCube.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace std;

Vector3::Vector3()
{

}

Vector3::Vector3(double x, double y, double z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3::Vector3(double splat)
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

bool Vector3::NearlyEquals(const Vector3& rh, double epsilon) const
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

double Vector3::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const double* ptr = &X;
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

double Vector3::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z;
}

double Vector3::Length_get() const
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

const double& Vector3::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const double* ptr = &X;
	return ptr[index];
}

double& Vector3::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	double* ptr = &X;
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

double Vector3::operator |(const Vector3& value) const
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

double Vector3::DistanceSq(const Vector3& left, const Vector3& right)
{
	return (right - left).LengthSq;
}

double Vector3::Distance(const Vector3& left, const Vector3& right)
{
	return (right - left).Length;
}

double Vector3::DotProduct(const Vector3& left, const Vector3& right)
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

Vector3 Numerics::operator +(double left, const Vector3& right)
{
	return Vector3(left) + right;
}

Vector3 Numerics::operator -(double left, const Vector3& right)
{
	return Vector3(left) - right;
}

Vector3 Numerics::operator *(double left, const Vector3& right)
{
	return Vector3(left) * right;
}

Vector3 Numerics::operator /(double left, const Vector3& right)
{
	return Vector3(left) / right;
}
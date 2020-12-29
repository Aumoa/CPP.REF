// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Vector4.h"

#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Vector3.h"

using namespace std;

Vector4::Vector4()
{

}

Vector4::Vector4(const Vector3& v3, double w) : This(v3.X, v3.Y, v3.Z, w)
{

}

Vector4::Vector4(double x, double y, double z, double w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Vector4::Vector4(double splat)
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

bool Vector4::NearlyEquals(const Vector4& rh, double epsilon) const
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

double Vector4::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const double* ptr = &X;
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

double Vector4::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

double Vector4::Length_get() const
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

const double& Vector4::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const double* ptr = &X;
	return ptr[index];
}

double& Vector4::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	double* ptr = &X;
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

double Vector4::operator |(const Vector4& value) const
{
	return DotProduct(*this, value);
}

bool Vector4::operator ==(const Vector4& value) const
{
	return X == value.X && Y == value.Y && Z == value.Z && W == value.W;
}

bool Vector4::operator !=(const Vector4& value) const
{
	return X != value.X || Y != value.Y || Z != value.Z || W != value.W;
}

bool Vector4::operator < (const Vector4& right) const
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
	else if (Z > right.Z)
	{
		return false;
	}
	else if (W < right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector4::operator <=(const Vector4& right) const
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
	else if (Z > right.Z)
	{
		return false;
	}
	else if (W <= right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector4::operator > (const Vector4& right) const
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
	else if (Z < right.Z)
	{
		return false;
	}
	else if (W > right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector4::operator >=(const Vector4& right) const
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
	else if (Z < right.Z)
	{
		return false;
	}
	else if (W >= right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Vector4::operator <=>(const Vector4& right) const
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
	else if (W < right.W)
	{
		return weak_ordering::less;
	}
	else if (W > right.W)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
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

double Vector4::DistanceSq(const Vector4& left, const Vector4& right)
{
	return (right - left).LengthSq;
}

double Vector4::Distance(const Vector4& left, const Vector4& right)
{
	return (right - left).Length;
}

double Vector4::DotProduct(const Vector4& left, const Vector4& right)
{
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
}

Vector4 operator +(double left, const Vector4& right)
{
	return Vector4(left) + right;
}

Vector4 operator -(double left, const Vector4& right)
{
	return Vector4(left) - right;
}

Vector4 operator *(double left, const Vector4& right)
{
	return Vector4(left) * right;
}

Vector4 operator /(double left, const Vector4& right)
{
	return Vector4(left) / right;
}